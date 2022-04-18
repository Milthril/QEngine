#include "QSkeletonModelComponent.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "ECS\QEntity.h"
#include "Asset\GAssetMgr.h"

SkeletonModelComponentSubMesh::SkeletonModelComponentSubMesh(QSkeletonModelComponent* model, const Asset::SkeletonModel::Mesh* mesh)
	: mMesh(mesh)
	, mModel(model) {
	updateMaterial();
}

SkeletonModelComponentSubMesh::~SkeletonModelComponentSubMesh() {
	mMaterial->removeRef(this);
}

void SkeletonModelComponentSubMesh::updateMaterial() {
	Asset::Material* material = mModel->mMaterialList[mMesh->mMaterialIndex].get();
	if (material != mMaterial) {
		if (mMaterial) {
			mMaterial->removeRef(this);
		}
		mMaterial = material;
		mMaterial->addRef(this);
		bNeedRecreatePipeline.active();
	}
}

void SkeletonModelComponentSubMesh::recreateResource() {
	if (!mMesh || !mMaterial|| !mModel)
		return;
	mVertexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Static, QRhiBuffer::VertexBuffer, sizeof(Asset::SkeletonModel::Vertex) * mMesh->mVertices.size()));
	mVertexBuffer->create();
	mIndexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Static, QRhiBuffer::IndexBuffer, sizeof(Asset::SkeletonModel::Index) * mMesh->mIndices.size()));
	mIndexBuffer->create();
}

void SkeletonModelComponentSubMesh::recreatePipeline() {
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
		)").arg(mModel->mSkeleton->getBoneOffsetMatrix().size());

	QShader vs = QRenderSystem::createShaderFromCode(QShader::Stage::VertexStage, vertexShaderCode.toLocal8Bit());
	if (!vs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}

	const QRhiUniformProxy::UniformInfo& materialInfo = mMaterial->getProxy()->getUniformInfo(1);

	QString defineCode = materialInfo.uniformDefineCode;
	QString outputCode = mMaterial->getShadingCode();
	if (QRenderSystem::instance()->isEnableDebug()) {
		defineCode.prepend("layout (location = 1) out vec4 CompId;\n");
		outputCode.append(QString("CompId = %1;\n").arg(mModel->getEntityIdVec4String()));
	}

	QString fragShaderCode = QString(R"(#version 440
		layout(location = 0) in vec2 vUV;
		layout(location = 0) out vec4 FragColor;
		%1
		void main(){
			%2
		}
		)").arg(defineCode).arg(outputCode);

	QShader fs = QRenderSystem::createShaderFromCode(QShader::Stage::FragmentStage, fragShaderCode.toLocal8Bit());
	if (!fs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}


	QVector<QRhiVertexInputBinding> inputBindings;
	inputBindings << QRhiVertexInputBinding{ sizeof(Asset::SkeletonModel::Vertex) };
	QVector<QRhiVertexInputAttribute> attributeList;
	attributeList << QRhiVertexInputAttribute{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(Asset::SkeletonModel::Vertex, position) };
	attributeList << QRhiVertexInputAttribute{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(Asset::SkeletonModel::Vertex,normal) };
	attributeList << QRhiVertexInputAttribute{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(Asset::SkeletonModel::Vertex,tangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(Asset::SkeletonModel::Vertex,bitangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 4, QRhiVertexInputAttribute::Float3, offsetof(Asset::SkeletonModel::Vertex,texCoord) };
	attributeList << QRhiVertexInputAttribute{ 0, 5, QRhiVertexInputAttribute::UInt4 , offsetof(Asset::SkeletonModel::Vertex,boneIndex) };
	attributeList << QRhiVertexInputAttribute{ 0, 6, QRhiVertexInputAttribute::Float4, offsetof(Asset::SkeletonModel::Vertex,boneWeight) };

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings(inputBindings.begin(), inputBindings.end());
	inputLayout.setAttributes(attributeList.begin(), attributeList.end());

	mPipeline.reset(RHI->newGraphicsPipeline());
	const auto& blendStates = QRenderSystem::instance()->getSceneBlendStates();
	mPipeline->setTargetBlends(blendStates.begin(), blendStates.end());
	mPipeline->setTopology(QRhiGraphicsPipeline::Topology::Triangles);
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(QRenderSystem::instance()->getSceneSampleCount());
	mPipeline->setVertexInputLayout(inputLayout);

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							   });

	mShaderResourceBindings.reset(RHI->newShaderResourceBindings());
	QVector<QRhiShaderResourceBinding> shaderBindings;
	shaderBindings << QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, mModel->mBoneMatBuffer.get());
	shaderBindings << materialInfo.bindings;
	mShaderResourceBindings->setBindings(shaderBindings.begin(), shaderBindings.end());
	mShaderResourceBindings->create();

	mPipeline->setShaderResourceBindings(mShaderResourceBindings.get());

	mPipeline->setRenderPassDescriptor(QRenderSystem::instance()->getSceneRenderPassDescriptor());

	mPipeline->create();
}

void SkeletonModelComponentSubMesh::uploadResource(QRhiResourceUpdateBatch* batch) {
	batch->uploadStaticBuffer(mVertexBuffer.get(),0 , sizeof(Asset::SkeletonModel::Vertex) * mMesh->mVertices.size(), mMesh->mVertices.data());
	batch->uploadStaticBuffer(mIndexBuffer.get(), 0, sizeof(Asset::SkeletonModel::Index) * mMesh->mIndices.size(), mMesh->mIndices.data());
}

void SkeletonModelComponentSubMesh::updateResourcePrePass(QRhiResourceUpdateBatch* batch) {
	if (bNeedRecreatePipeline.receive()) {
		recreatePipeline();
	}
}

void SkeletonModelComponentSubMesh::renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	if (!mPipeline)
		return;
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mMesh->mIndices.size());
}


void QSkeletonModelComponent::recreateResource() {
	if (!mSkeletonModel || !mSkeleton || mMaterialList.isEmpty())
		return;
	mSkeletonSubMeshList.clear();
	for (auto& mesh : mSkeletonModel->getMeshList()) {
		mSkeletonSubMeshList << std::make_shared<SkeletonModelComponentSubMesh>(this, &mesh);
	}

	for (auto& subMesh : mSkeletonSubMeshList){
		subMesh->recreateResource();
	}

	mBoneMatBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(float) * 16 + mSkeleton->getBoneOffsetMatrix().size() * sizeof(Asset::Skeleton::Mat4)));
	mBoneMatBuffer->create();
}

void QSkeletonModelComponent::recreatePipeline() {
	if (!mSkeletonModel || !mSkeleton || mMaterialList.isEmpty())
		return;
	for (auto& subMesh : mSkeletonSubMeshList) {
		subMesh->recreatePipeline();
	}
}

void QSkeletonModelComponent::uploadResource(QRhiResourceUpdateBatch* batch) {

	if (!mSkeletonModel || !mSkeleton || mMaterialList.isEmpty())
		return;
	for (auto& subMesh : mSkeletonSubMeshList) {
		subMesh->uploadResource(batch);
	}
}

void QSkeletonModelComponent::updateResourcePrePass(QRhiResourceUpdateBatch* batch) {
	if (!mSkeletonModel || !mSkeleton || mMaterialList.isEmpty())
		return;
	for (auto& material : mMaterialList) {
		material->getProxy()->updateResource(batch);
	}
	for (auto& subMesh : mSkeletonSubMeshList) {
		subMesh->updateResourcePrePass(batch);
	}

	QMatrix4x4 MVP = mEntity->calculateMatrixMVP();
	batch->updateDynamicBuffer(mBoneMatBuffer.get(), 0, sizeof(float) * 16, MVP.constData());
	const auto& posesMatrix = mSkeleton->getCurrentPosesMatrix();
	batch->updateDynamicBuffer(mBoneMatBuffer.get(), sizeof(float) * 16, sizeof(Asset::Skeleton::Mat4) * posesMatrix.size(), posesMatrix.constData());
}

void QSkeletonModelComponent::renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	if (!mSkeletonModel || !mSkeleton || mMaterialList.isEmpty())
		return;
	for (auto& subMesh : mSkeletonSubMeshList) {
		subMesh->renderInPass(cmdBuffer, viewport);
	}
}

std::shared_ptr<Asset::SkeletonModel> QSkeletonModelComponent::getSkeletonModel() const {
	return mSkeletonModel;
}

void QSkeletonModelComponent::setSkeletonModel(std::shared_ptr<Asset::SkeletonModel> val) {
	mSkeletonModel = val;
	if (mSkeletonModel) {
		mSkeleton = TheAssetMgr->load<Asset::Skeleton>(mSkeletonModel->getSkeletonPath());
		mMaterialList.resize(mSkeletonModel->getMaterialPathList().size());
		for (int i = 0; i < mMaterialList.size(); i++) {
			mMaterialList[i] = TheAssetMgr->load<Asset::Material>(mSkeletonModel->getMaterialPathList()[i]);
		}
	}
	bNeedRecreateResource.active();
	bNeedRecreatePipeline.active();
}

const QVector<std::shared_ptr<Asset::Material>>& QSkeletonModelComponent::getMaterialList() const {
	return mMaterialList;
}

void QSkeletonModelComponent::setMaterialList(QVector<std::shared_ptr<Asset::Material>> val) {
	mMaterialList = val;
	for (auto& subMesh : mSkeletonSubMeshList) {
		subMesh->updateMaterial();
	}
}
