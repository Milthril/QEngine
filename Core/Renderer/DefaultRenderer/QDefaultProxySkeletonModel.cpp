#include "Renderer\ISceneRenderPass.h"
#include "QDefaultProxySkeletonModel.h"
#include "QEngine.h"
#include "Scene\Component\QPrimitiveComponent.h"
#include "Scene\Component\SkeletonMesh\QSkeletonMeshComponent.h"

QDefaultProxySkeletonModel::QDefaultProxySkeletonModel(std::shared_ptr<QSkeletonModelComponent> mesh)
	:mSkeletonModel(mesh)
{
}

void QDefaultProxySkeletonModel::recreateResource()
{
	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(QMatrix4x4)));
	mUniformBuffer->create();

	int indexOffset = 0;
	int vertexOffset = 0;
	const auto& meshes = mSkeletonModel->getMeshes();
	mMeshProxyList.resize(meshes.size());
	for (int i = 0; i < mMeshProxyList.size(); i++) {
		auto& meshProxy = mMeshProxyList[i];
		const auto& mesh = meshes[i];
		meshProxy = std::make_shared<SkeletonMeshProxy>();
		meshProxy->mesh = mesh;
		meshProxy->indexOffset = indexOffset;
		meshProxy->indexRange = mesh->getIndices().size();
		indexOffset += meshProxy->indexRange;
		meshProxy->vertexOffset = vertexOffset;
		meshProxy->vertexRange = mesh->getVertices().size();
		vertexOffset += meshProxy->vertexRange;
	}
	mVertexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Immutable, QRhiBuffer::VertexBuffer, sizeof(QSkeletonModelComponent::Vertex) * vertexOffset));
	mVertexBuffer->create();

	mIndexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Immutable, QRhiBuffer::IndexBuffer, sizeof(QSkeletonModelComponent::Index) * indexOffset));
	mIndexBuffer->create();

	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(float) * 16 + mSkeletonModel->getSkeleton()->getBoneMatrix().size() * sizeof(QSkeleton::Mat4)));
	mUniformBuffer->create();
}

void QDefaultProxySkeletonModel::recreatePipeline()
{
	for (int i = 0; i < mMeshProxyList.size(); i++) {
		auto& meshProxy = mMeshProxyList[i];
		auto& pipeline = mMeshProxyList[i]->pipeline;
		pipeline.reset(RHI->newGraphicsPipeline());

		const auto& blendStates = mRenderPass->getBlendStates();
		pipeline->setTargetBlends(blendStates.begin(), blendStates.end());

		pipeline->setTopology(QRhiGraphicsPipeline::Topology::Triangles);
		pipeline->setDepthTest(true);
		pipeline->setDepthWrite(true);
		pipeline->setSampleCount(mRenderPass->getSampleCount());

		QVector<QRhiVertexInputBinding> inputBindings;
		inputBindings << QRhiVertexInputBinding{ sizeof(QSkeletonModelComponent::Vertex) };
		QVector<QRhiVertexInputAttribute> attributeList;
		attributeList << QRhiVertexInputAttribute{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(QSkeletonModelComponent::Vertex, position) };
		attributeList << QRhiVertexInputAttribute{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(QSkeletonModelComponent::Vertex,normal) };
		attributeList << QRhiVertexInputAttribute{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(QSkeletonModelComponent::Vertex,tangent) };
		attributeList << QRhiVertexInputAttribute{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(QSkeletonModelComponent::Vertex,bitangent) };
		attributeList << QRhiVertexInputAttribute{ 0, 4, QRhiVertexInputAttribute::Float3, offsetof(QSkeletonModelComponent::Vertex,texCoord) };
		attributeList << QRhiVertexInputAttribute{ 0, 5, QRhiVertexInputAttribute::UInt4 , offsetof(QSkeletonModelComponent::Vertex,boneIndex) };
		attributeList << QRhiVertexInputAttribute{ 0, 6, QRhiVertexInputAttribute::Float4, offsetof(QSkeletonModelComponent::Vertex,boneWeight) };

		QString vertexShaderCode = QString(R"(#version 440
			layout(location = 0) in vec3 inPosition;
			layout(location = 1) in vec3 inNormal;
			layout(location = 2) in vec3 inTangent;
			layout(location = 3) in vec3 inBitangent;
			layout(location = 4) in vec2 inUV;
			layout(location = 5) in uvec4 boneIndex;
			layout(location = 6) in vec4 boneWeight;

			layout(location = 0) out vec2 vUV;

			out gl_PerVertex{
				vec4 gl_Position;
			};

			layout(std140,binding = 0) uniform buf{
				mat4 mvp;
				mat4 boneMatrix[%1];
			}ubuf;

			void main(){
				mat4 BoneTransform =  ubuf.boneMatrix[boneIndex[0]]*boneWeight[0];
					 BoneTransform += ubuf.boneMatrix[boneIndex[1]]*boneWeight[1];
					 BoneTransform += ubuf.boneMatrix[boneIndex[2]]*boneWeight[2];
					 BoneTransform += ubuf.boneMatrix[boneIndex[3]]*boneWeight[3];

				vUV = inUV;
				gl_Position = ubuf.mvp * BoneTransform * vec4(inPosition,1.0f);
			}
		)").arg(mSkeletonModel->getSkeleton()->getBoneMatrix().size());

		QRhiVertexInputLayout inputLayout;
		inputLayout.setBindings(inputBindings.begin(), inputBindings.end());
		inputLayout.setAttributes(attributeList.begin(), attributeList.end());
		pipeline->setVertexInputLayout(inputLayout);

		QShader vs = ISceneRenderer::createShaderFromCode(QShader::Stage::VertexStage, vertexShaderCode.toLocal8Bit());
		const QRhiUniformProxy::UniformInfo& materialInfo = meshProxy->mesh->getMaterial()->getProxy()->getUniformInfo(1);

		QString defineCode = materialInfo.uniformDefineCode;
		QString outputCode = meshProxy->mesh->getMaterial()->getShadingCode();
		if (mRenderPass->getEnableOutputDebugId()) {
			defineCode.prepend("layout (location = 1) out vec4 CompId;\n");
			outputCode.append(QString("CompId = %1;\n").arg(mSkeletonModel->getCompIdVec4String()));
		}

		QString fragShaderCode = QString(R"(#version 440
		layout(location = 0) in vec2 vUV;
		layout(location = 0) out vec4 FragColor;
		%1
		void main(){
			%2
		}
		)").arg(defineCode).arg(outputCode);
		QShader fs = ISceneRenderer::createShaderFromCode(QShader::Stage::FragmentStage, fragShaderCode.toLocal8Bit());
		Q_ASSERT(fs.isValid());

		pipeline->setShaderStages({
			{ QRhiShaderStage::Vertex, vs },
			{ QRhiShaderStage::Fragment, fs }
								  });
		auto& shaderResBindings = mMeshProxyList[i]->shaderBindings;
		shaderResBindings.reset(RHI->newShaderResourceBindings());

		QVector<QRhiShaderResourceBinding> shaderBindings;
		shaderBindings << QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, mUniformBuffer.get());
		shaderBindings << materialInfo.bindings;
		shaderResBindings->setBindings(shaderBindings.begin(), shaderBindings.end());

		shaderResBindings->create();

		pipeline->setShaderResourceBindings(shaderResBindings.get());

		pipeline->setRenderPassDescriptor(mRenderPass->getRenderPassDescriptor());

		pipeline->create();
	}
}

void QDefaultProxySkeletonModel::uploadResource(QRhiResourceUpdateBatch* batch)
{
	for (int i = 0; i < mMeshProxyList.size(); i++) {
		batch->uploadStaticBuffer(mVertexBuffer.get(), sizeof(QSkeletonModelComponent::Vertex) * mMeshProxyList[i]->vertexOffset, sizeof(QSkeletonModelComponent::Vertex) * mMeshProxyList[i]->vertexRange, mMeshProxyList[i]->mesh->getVertices().data());
		batch->uploadStaticBuffer(mIndexBuffer.get(), sizeof(QSkeletonModelComponent::Index) * mMeshProxyList[i]->indexOffset, sizeof(QSkeletonModelComponent::Index) * mMeshProxyList[i]->indexRange, mMeshProxyList[i]->mesh->getIndices().data());
	}
}

void QDefaultProxySkeletonModel::updateResource(QRhiResourceUpdateBatch* batch) {

	for (auto& material : mSkeletonModel->getMaterialList()) {
		material->getProxy()->updateResource(batch);
	}
	QMatrix4x4 MVP = mSkeletonModel->calculateMVP();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(float) * 16, MVP.constData());
	const auto& posesMatrix = mSkeletonModel->getSkeleton()->getCurrentPosesMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), sizeof(float) * 16, sizeof(QSkeleton::Mat4) * posesMatrix.size(), posesMatrix.constData());
}

void QDefaultProxySkeletonModel::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	for (auto& meshProxy : mMeshProxyList) {
		cmdBuffer->setGraphicsPipeline(meshProxy->pipeline.get());
		cmdBuffer->setViewport(viewport);
		cmdBuffer->setShaderResources();
		const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), meshProxy->vertexOffset * sizeof(QSkeletonModelComponent::Vertex));
		cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), meshProxy->indexOffset * sizeof(QSkeletonModelComponent::Index), QRhiCommandBuffer::IndexUInt32);
		cmdBuffer->drawIndexed(meshProxy->indexRange);
	}
}