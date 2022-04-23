#include "IShapeComponent.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "ECS\QEntity.h"

void IShapeComponent::setVertices(const QVector<Vertex>&& val) {
	if (mVertices.size() != val.size()) 
		bNeedRecreateResource.active();
	bNeedUpdateVertices.active();
	mVertices = val;
}

void IShapeComponent::setIndices(const QVector<Index>&& val) {
	if (mIndices.size() == val.size()) 
		bNeedUpdateIndices.active();
	else 
		bNeedRecreateResource.active();
	mIndices = val;
}

void IShapeComponent::setSimpleVertex(const QVector<SimpleVertex>& val) {
	QVector<Vertex> vertices(val.size());
	for (int i = 0; i < val.size(); i++) {
		vertices[i].position = val[i].position;
		vertices[i].texCoord = val[i].texCoord;
	}

	QVector<Index> indices(vertices.size());
	for (int i = 0; i < vertices.size(); i++) 
		indices[i] = i;
	
	for (int i = 0; i < vertices.size() - 3; i+=3) {
		QVector3D v = vertices[i + 1].position - vertices[i].position;
		QVector3D w = vertices[i + 2].position - vertices[i].position;

		QVector3D normal = QVector3D::crossProduct(v, w).normalized();

		float sx = vertices[i + 1].texCoord.x() - vertices[i].texCoord.x(), sy = vertices[i + 1].texCoord.y() - vertices[i].texCoord.y();
		float tx = vertices[i + 2].texCoord.x() - vertices[i].texCoord.x(), ty = vertices[i + 2].texCoord.y() - vertices[i].texCoord.y();
		float dirCorrection = (tx * sy - ty * sx) < 0.0f ? -1.0f : 1.0f;
		if (sx * ty == sy * tx) {
			sx = 0.0;
			sy = 1.0;
			tx = 1.0;
			ty = 0.0;
		}
		QVector3D tangent, bitangent;
		tangent.setX((w.x() * sy - v.x() * ty) * dirCorrection);
		tangent.setY((w.y() * sy - v.y() * ty) * dirCorrection);
		tangent.setZ((w.z() * sy - v.z() * ty) * dirCorrection);
		bitangent.setX((-w.x() * sx + v.x() * tx) * dirCorrection);
		bitangent.setY((-w.y() * sx + v.y() * tx) * dirCorrection);
		bitangent.setZ((-w.z() * sx + v.z() * tx) * dirCorrection);

		vertices[i].normal = vertices[i + 1].normal = vertices[i + 2].normal = normal;
		vertices[i].tangent = vertices[i + 1].tangent = vertices[i + 2].tangent = tangent;
		vertices[i].bitangent = vertices[i + 1].bitangent = vertices[i + 2].bitangent = normal;
	}
	setIndices(std::move(indices));
	setVertices(std::move(vertices));
}

void IShapeComponent::setMaterial(std::shared_ptr<Asset::Material> val) {
	if (mMaterial) {
		mMaterial->removeRef(this);
	}
	mMaterial = val;
	if (val) {
		val->addRef(this);
	}
	bNeedRecreatePipeline.active();
}


void IShapeComponent::recreateResource() {
	if (mVertices.isEmpty() || mIndices.isEmpty() || !mMaterial)
		return;
	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(UniformMatrix)));
	mUniformBuffer->create();

	mVertexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::VertexBuffer, sizeof(Vertex) * mVertices.size()));
	mVertexBuffer->create();

	mIndexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::IndexBuffer, sizeof(Index) * mIndices.size()));
	mIndexBuffer->create();
}

void IShapeComponent::recreatePipeline() {
	if (mVertices.isEmpty() || mIndices.isEmpty() || !mMaterial)
		return;
	QVector<QRhiVertexInputBinding> inputBindings;
	inputBindings << QRhiVertexInputBinding{ sizeof(Vertex) };
	QVector<QRhiVertexInputAttribute> attributeList;
	attributeList << QRhiVertexInputAttribute{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(Vertex, position) };
	attributeList << QRhiVertexInputAttribute{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(Vertex,normal) };
	attributeList << QRhiVertexInputAttribute{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(Vertex,tangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(Vertex,bitangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 4, QRhiVertexInputAttribute::Float2, offsetof(Vertex,texCoord) };

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

void IShapeComponent::uploadResource(QRhiResourceUpdateBatch* batch) {

}

void IShapeComponent::updatePrePass(QRhiCommandBuffer* cmdBuffer) {

}

void IShapeComponent::updateResourcePrePass(QRhiResourceUpdateBatch* batch) {
	if (mVertices.isEmpty() || mIndices.isEmpty()|| !mMaterial)
		return;
	if (bNeedUpdateVertices.receive()) {
		batch->updateDynamicBuffer(mVertexBuffer.get(), 0, sizeof(Vertex) * mVertices.size(), mVertices.data());
	}
	if (bNeedUpdateIndices.receive()) {
		batch->updateDynamicBuffer(mIndexBuffer.get(), 0, sizeof(Index) * mIndices.size(), mIndices.data());
	}
	UniformMatrix Matrix;
	Matrix.MVP = mEntity->calculateMatrixMVP().toGenericMatrix<4, 4>();
	Matrix.M = mEntity->calculateMatrixModel().toGenericMatrix<4,4>();
	mMaterial->updateResource(batch);
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(UniformMatrix), &Matrix);
}

void IShapeComponent::renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	if (!mPipeline)
		return;
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mIndices.size());
}

bool IShapeComponent::isDefer() {
	return true;
}
