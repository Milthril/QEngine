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
			layout(location = 1) out vec3 vWorldPosition;
			layout(location = 2) out mat3 vTangentBasis;

			out gl_PerVertex{
				vec4 gl_Position;
			};

			layout(std140,binding = 0) uniform buf{
				mat4 MVP;
				mat4 M;
				mat4 boneMatrix[%1];
			}ubuf;

			void main(){
				mat4 BoneTransform =  ubuf.boneMatrix[boneIndex[0]]*boneWeight[0];
					 BoneTransform += ubuf.boneMatrix[boneIndex[1]]*boneWeight[1];
					 BoneTransform += ubuf.boneMatrix[boneIndex[2]]*boneWeight[2];
					 BoneTransform += ubuf.boneMatrix[boneIndex[3]]*boneWeight[3];
					
				vUV = inUV;
				vec4 pos = BoneTransform * vec4(inPosition,1.0f);
				vWorldPosition = vec3(ubuf.M * pos);
				vTangentBasis =  mat3(ubuf.M) * mat3(inTangent, inBitangent, inNormal);
				gl_Position = ubuf.MVP * pos;
			}
		)").arg(mModel->mSkeleton->getBoneOffsetMatrix().size());

	QShader vs = QRenderSystem::createShaderFromCode(QShader::Stage::VertexStage, vertexShaderCode.toLocal8Bit());
	if (!vs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}

	const QRhiUniform::UniformInfo& materialInfo = mMaterial->getUniformInfo(1);

	QString defineCode = materialInfo.uniformDefineCode;
	QString outputCode = mMaterial->getShadingCode();
	if (TheRenderSystem->isEnableDebug()) {
		defineCode.prepend("layout (location = 4) out vec4 CompId;\n");
		outputCode.append(QString("CompId = %1;\n").arg(mModel->getEntityIdVec4String()));
	}

	QString fragShaderCode = QString(R"(#version 440
		layout(location = 0) in vec2 vUV;
		layout(location = 1) in vec3 vWorldPosition;
		layout(location = 2) in mat3 vTangentBasis;

		layout(location = 0) out vec4 outBaseColor;
		layout(location = 1) out vec4 outPosition;
		layout(location = 2) out vec4 outNormal_Metalness;
		layout(location = 3) out vec4 outTangent_Roughness;
		%1
		#define outNormal		(outNormal_Metalness.rgb)
		#define outMetalness	(outNormal_Metalness.a)
		#define outTangent		(outTangent_Roughness.rgb)
		#define outRoughness	(outTangent_Roughness.a)
		void main(){
			outPosition = vec4(vWorldPosition,1.0);
			outTangent = vTangentBasis[0];
			outNormal = vTangentBasis[2];
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
	attributeList << QRhiVertexInputAttribute{ 0, 4, QRhiVertexInputAttribute::Float2, offsetof(Asset::SkeletonModel::Vertex,texCoord) };
	attributeList << QRhiVertexInputAttribute{ 0, 5, QRhiVertexInputAttribute::UInt4 , offsetof(Asset::SkeletonModel::Vertex,boneIndex) };
	attributeList << QRhiVertexInputAttribute{ 0, 6, QRhiVertexInputAttribute::Float4, offsetof(Asset::SkeletonModel::Vertex,boneWeight) };

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings(inputBindings.begin(), inputBindings.end());
	inputLayout.setAttributes(attributeList.begin(), attributeList.end());

	mPipeline.reset(RHI->newGraphicsPipeline());
	const auto& blendStates = TheRenderSystem->renderer()->getDeferPassBlendStates();
	mPipeline->setTargetBlends(blendStates.begin(), blendStates.end());
	mPipeline->setTopology(QRhiGraphicsPipeline::Topology::Triangles);
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(TheRenderSystem->renderer()->getDeferPassSampleCount());
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

	mPipeline->setRenderPassDescriptor(TheRenderSystem->renderer()->getDeferPassDescriptor());

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
	uint64_t size = sizeof(UniformMatrix) + mSkeleton->getBoneOffsetMatrix().size() * sizeof(Asset::Skeleton::Mat4);
	mBoneMatBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(UniformMatrix) + mSkeleton->getBoneOffsetMatrix().size() * sizeof(Asset::Skeleton::Mat4)));
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
		material->updateResource(batch);
	}
	for (auto& subMesh : mSkeletonSubMeshList) {
		subMesh->updateResourcePrePass(batch);
	}

	if (mAnimation) {
		mAnimation->play(mSkeleton.get(), QTime::currentTime().msecsSinceStartOfDay()/1000.0);
	}

	QMatrix4x4 MVP = mEntity->calculateMatrixMVP();
	QMatrix4x4 M = mEntity->calculateMatrixModel();
	batch->updateDynamicBuffer(mBoneMatBuffer.get(), 0, sizeof(float) * 16, MVP.constData());
	batch->updateDynamicBuffer(mBoneMatBuffer.get(), sizeof(float) * 16, sizeof(float) * 16, M.constData());
	const auto& posesMatrix = mSkeleton->getCurrentPosesMatrix();
	batch->updateDynamicBuffer(mBoneMatBuffer.get(), sizeof(float) * 16 * 2, sizeof(Asset::Skeleton::Mat4) * posesMatrix.size(), posesMatrix.constData());
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
		mSkeletonSubMeshList.clear();
		mSkeleton = TheAssetMgr->load<Asset::Skeleton>(mSkeletonModel->getSkeletonPath());
		mMaterialList.resize(mSkeletonModel->getMaterialPathList().size());
		for (int i = 0; i < mMaterialList.size(); i++) {
			mMaterialList[i] = TheAssetMgr->load<Asset::Material>(mSkeletonModel->getMaterialPathList()[i]);
		}
	}
	bNeedRecreateResource.active();
	bNeedRecreatePipeline.active();
}

std::shared_ptr<Asset::SkeletonAnimation> QSkeletonModelComponent::getAnimation() const {
	return mAnimation;
}

void QSkeletonModelComponent::setAnimation(std::shared_ptr<Asset::SkeletonAnimation> val) {
	mAnimation = val;
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

bool QSkeletonModelComponent::isDefer() {
	return true;
}
