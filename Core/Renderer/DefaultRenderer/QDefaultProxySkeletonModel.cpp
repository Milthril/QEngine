#include "Renderer\ISceneRenderPass.h"
#include "QDefaultProxySkeletonModel.h"
#include "QEngine.h"
#include "Scene\Component\QPrimitiveComponent.h"
#include "Scene\Component\SkeletonMesh\QSkeletonMeshComponent.h"


void QDefaultProxySkeletonMesh::recreateResource() {
	throw std::logic_error("The method or operation is not implemented.");
}

void QDefaultProxySkeletonMesh::recreatePipeline() {
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
		)").arg(mModel->mSkeletonModel->getSkeleton()->getBoneMatrix().size());


	QShader vs = ISceneRenderer::createShaderFromCode(QShader::Stage::VertexStage, vertexShaderCode.toLocal8Bit());	
	if (!vs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}
	const QRhiUniformProxy::UniformInfo& materialInfo = mMesh->getMaterial()->getProxy()->getUniformInfo(1);

	QString defineCode = materialInfo.uniformDefineCode;
	QString outputCode = mMesh->getMaterial()->getShadingCode();
	if (mRenderPass->getEnableOutputDebugId()) {
		defineCode.prepend("layout (location = 1) out vec4 CompId;\n");
		outputCode.append(QString("CompId = %1;\n").arg(mModel->mSkeletonModel->getCompIdVec4String()));
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
	if (!fs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}


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

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings(inputBindings.begin(), inputBindings.end());
	inputLayout.setAttributes(attributeList.begin(), attributeList.end());

	mPipeline.reset(RHI->newGraphicsPipeline());
	const auto& blendStates = mRenderPass->getBlendStates();
	mPipeline->setTargetBlends(blendStates.begin(), blendStates.end());
	mPipeline->setTopology(QRhiGraphicsPipeline::Topology::Triangles);
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(mRenderPass->getSampleCount());
	mPipeline->setVertexInputLayout(inputLayout);

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							  });
	mShaderBindings.reset(RHI->newShaderResourceBindings());

	QVector<QRhiShaderResourceBinding> shaderBindings;
	shaderBindings << QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage,mModel->mUniformBuffer.get());
	shaderBindings << materialInfo.bindings;
	mShaderBindings->setBindings(shaderBindings.begin(), shaderBindings.end());

	mShaderBindings->create();

	mPipeline->setShaderResourceBindings(mShaderBindings.get());

	mPipeline->setRenderPassDescriptor(mRenderPass->getRenderPassDescriptor());

	mPipeline->create();
}

void QDefaultProxySkeletonMesh::uploadResource(QRhiResourceUpdateBatch* batch) {
	batch->uploadStaticBuffer(mVertexBuffer.get(), sizeof(QSkeletonModelComponent::Vertex) *mVertexOffset, sizeof(QSkeletonModelComponent::Vertex) * mVertexRange, mMesh->getVertices().data());
	batch->uploadStaticBuffer(mIndexBuffer.get(), sizeof(QSkeletonModelComponent::Index) * mIndexOffset, sizeof(QSkeletonModelComponent::Index) * mIndexRange, mMesh->getIndices().data());
}

void QDefaultProxySkeletonMesh::updateResource(QRhiResourceUpdateBatch* batch) {
	if (mMesh->bNeedRecreatePipeline.receive()) {
		recreatePipeline();
	}
}

void QDefaultProxySkeletonMesh::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	if (!mPipeline)
		return;
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), mVertexOffset * sizeof(QSkeletonModelComponent::Vertex));
	cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), mIndexOffset * sizeof(QSkeletonModelComponent::Index), QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mIndexRange);
}

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
		meshProxy = std::make_shared<QDefaultProxySkeletonMesh>();
		meshProxy->mMesh = mesh;
		meshProxy->mModel = this;
		meshProxy->mRenderPass = mRenderPass;
		meshProxy->mIndexOffset = indexOffset;
		meshProxy->mIndexRange = mesh->getIndices().size();

		indexOffset += meshProxy->mIndexRange;
		meshProxy->mVertexOffset = vertexOffset;
		meshProxy->mVertexRange = mesh->getVertices().size();
		vertexOffset += meshProxy->mVertexRange;
	}

	mVertexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Immutable, QRhiBuffer::VertexBuffer, sizeof(QSkeletonModelComponent::Vertex) * vertexOffset));
	mVertexBuffer->create();

	mIndexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Immutable, QRhiBuffer::IndexBuffer, sizeof(QSkeletonModelComponent::Index) * indexOffset));
	mIndexBuffer->create();

	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(float) * 16 + mSkeletonModel->getSkeleton()->getBoneMatrix().size() * sizeof(QSkeleton::Mat4)));
	mUniformBuffer->create();

	for (int i = 0; i < mMeshProxyList.size(); i++) {
		auto& meshProxy = mMeshProxyList[i];
		meshProxy->mIndexBuffer = mIndexBuffer;
		meshProxy->mVertexBuffer = mVertexBuffer;
	}
}

void QDefaultProxySkeletonModel::recreatePipeline()
{
	for (auto& meshProxy : mMeshProxyList) {
		meshProxy->recreatePipeline();
	}
}

void QDefaultProxySkeletonModel::uploadResource(QRhiResourceUpdateBatch* batch)
{
	for (auto& meshProxy : mMeshProxyList) {
		meshProxy->uploadResource(batch);
	}
}

void QDefaultProxySkeletonModel::updateResource(QRhiResourceUpdateBatch* batch) {

	for (auto& material : mSkeletonModel->getMaterialList()) {
		material->getProxy()->updateResource(batch);
	}
	for (int i = 0; i < mMeshProxyList.size(); i++) {
		mMeshProxyList[i]->updateResource(batch);
	}
	QMatrix4x4 MVP = mSkeletonModel->calculateMVP();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(float) * 16, MVP.constData());
	const auto& posesMatrix = mSkeletonModel->getSkeleton()->getCurrentPosesMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), sizeof(float) * 16, sizeof(QSkeleton::Mat4) * posesMatrix.size(), posesMatrix.constData());
}

void QDefaultProxySkeletonModel::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	for (auto& meshProxy : mMeshProxyList) {
		meshProxy->drawInPass(cmdBuffer, viewport);
	}
}
