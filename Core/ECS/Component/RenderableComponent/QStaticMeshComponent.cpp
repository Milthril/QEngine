#include "QStaticMeshComponent.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "ECS\QEntity.h"
#include "Asset\GAssetMgr.h"

void QStaticMeshComponent::setStaticMesh(std::shared_ptr<Asset::StaticMesh> val) {
	mStaticMesh = val;
	if (mStaticMesh) {
		setMaterial(TheAssetMgr->load<Asset::Material>(mStaticMesh->getMaterialPath()));
	}
	bNeedRecreatePipeline.active();
	bNeedRecreateResource.active();
}

void QStaticMeshComponent::setMaterial(std::shared_ptr<Asset::Material> val) {
	if (mMaterial) {
		mMaterial->removeRef(this);
	}
	mMaterial = val;
	if (val&&mStaticMesh) {
		mStaticMesh->setMaterialPath(val->getRefPath());
		val->addRef(this);
	}
	bNeedRecreatePipeline.active();
}

void QStaticMeshComponent::recreateResource() {
	if (!mStaticMesh || !mMaterial)
		return;
	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(UniformMatrix)));
	mUniformBuffer->create();

	mVertexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Static, QRhiBuffer::VertexBuffer, sizeof(Asset::StaticMesh::Vertex) * mStaticMesh->getVertices().size()));
	mVertexBuffer->create();

	mIndexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Static, QRhiBuffer::IndexBuffer, sizeof(Asset::StaticMesh::Index) * mStaticMesh->getIndices().size()));
	mIndexBuffer->create();
}

void QStaticMeshComponent::recreatePipeline() {
	if (!mStaticMesh||!mMaterial)
		return;
	if (mStaticMesh->getVertices().size() == 0) {
		return;
	}
	QVector<QRhiVertexInputBinding> inputBindings;
	inputBindings << QRhiVertexInputBinding{ sizeof(Asset::StaticMesh::Vertex) };
	QVector<QRhiVertexInputAttribute> attributeList;
	attributeList << QRhiVertexInputAttribute{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex, position) };
	attributeList << QRhiVertexInputAttribute{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex,normal) };
	attributeList << QRhiVertexInputAttribute{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex,tangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex,bitangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 4, QRhiVertexInputAttribute::Float2, offsetof(Asset::StaticMesh::Vertex,texCoord) };

	QString vertexShaderCode = R"(#version 440
	layout(location = 0) in vec3 inPosition;
	layout(location = 1) in vec3 inNormal;
	layout(location = 2) in vec3 inTangent;
	layout(location = 3) in vec3 inBitangent;
	layout(location = 4) in vec2 inUV;

	layout(location = 0) out vec2 vUV;
	layout(location = 1) out vec3 vWorldPosition;
	layout(location = 2) out mat3 vTangentBasis;

	out gl_PerVertex{
		vec4 gl_Position;
	};

	layout(std140,binding = 0) uniform UnifromMatrix{
		mat4 MVP;
		mat4 M;
	}Matrix;
	void main(){
		vUV = inUV;
		vWorldPosition = vec3(Matrix.M * vec4(inPosition,1.0f));
		vTangentBasis =  mat3(Matrix.M) * mat3(inTangent, inBitangent, inNormal);
		gl_Position = Matrix.MVP * vec4(inPosition,1.0f);
	}
	)";

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings(inputBindings.begin(), inputBindings.end());
	inputLayout.setAttributes(attributeList.begin(), attributeList.end());
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
		outputCode.append(QString("\nCompId = %1;\n").arg(getEntityIdVec4String()));
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

	mPipeline.reset(RHI->newGraphicsPipeline());
	
	mPipeline->setVertexInputLayout(inputLayout);

	const auto& blendStates = TheRenderSystem->renderer()->getDeferPassBlendStates();
	mPipeline->setTargetBlends(blendStates.begin(), blendStates.end());
	mPipeline->setTopology(QRhiGraphicsPipeline::Topology::Triangles);
	mPipeline->setDepthOp(QRhiGraphicsPipeline::LessOrEqual);
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(TheRenderSystem->renderer()->getDeferPassSampleCount());

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							   });
	mShaderResourceBindings.reset(RHI->newShaderResourceBindings());
	QVector<QRhiShaderResourceBinding> shaderBindings;
	shaderBindings << QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, mUniformBuffer.get());
	shaderBindings << materialInfo.bindings;
	mShaderResourceBindings->setBindings(shaderBindings.begin(), shaderBindings.end());
	mShaderResourceBindings->create();

	mPipeline->setShaderResourceBindings(mShaderResourceBindings.get());

	mPipeline->setRenderPassDescriptor(TheRenderSystem->renderer()->getDeferPassDescriptor());

	mPipeline->create();
}

void QStaticMeshComponent::uploadResource(QRhiResourceUpdateBatch* batch) {
	if (!mStaticMesh || !mMaterial)
		return;
	batch->uploadStaticBuffer(mVertexBuffer.get(), mStaticMesh->getVertices().constData());
	batch->uploadStaticBuffer(mIndexBuffer.get(), mStaticMesh->getIndices().constData());
}

void QStaticMeshComponent::updatePrePass(QRhiCommandBuffer* cmdBuffer) {
}

void QStaticMeshComponent::updateResourcePrePass(QRhiResourceUpdateBatch* batch) {
	if (!mStaticMesh || !mMaterial)
		return;
	mMaterial->updateResource(batch);

	UniformMatrix Matrix;
	Matrix.MVP = mEntity->calculateMatrixMVP().toGenericMatrix<4, 4>();
	Matrix.M = mEntity->calculateMatrixModel().toGenericMatrix<4,4>();

	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(UniformMatrix), &Matrix);
}

void QStaticMeshComponent::renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	if (!mPipeline)
		return;
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mStaticMesh->getIndices().size());
}

bool QStaticMeshComponent::isDefer() {
	return true;
}
