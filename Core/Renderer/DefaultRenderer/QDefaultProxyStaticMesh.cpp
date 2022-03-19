#include "QDefaultProxyStaticMesh.h"
#include "Scene\Component\QPrimitiveComponent.h"
#include "Scene\Component\StaticMesh\QStaticMeshComponent.h"
#include "QEngine.h"


QDefaultProxyStaticMesh::QDefaultProxyStaticMesh(std::shared_ptr<QStaticMeshComponent> mesh, bool usedByParticle)
	: mStaticMesh(mesh)
	, bUsedByParticle(usedByParticle)
{

}

void QDefaultProxyStaticMesh::recreateResource()
{
	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(QMatrix4x4)));
	Q_ASSERT(mUniformBuffer->create());

	mVertexBuffer.reset(RHI->newBuffer(mStaticMesh->getBufferType(), QRhiBuffer::VertexBuffer, sizeof(QStaticMeshComponent::Vertex) * mStaticMesh->getVertexCount()));
	Q_ASSERT(mVertexBuffer->create());

	if (mStaticMesh->getIndexedCount() == 0) {
		mStaticMesh->autoFillIndices();
	}
	mIndexBuffer.reset(RHI->newBuffer(mStaticMesh->getBufferType(), QRhiBuffer::IndexBuffer, sizeof(QStaticMeshComponent::Index) * mStaticMesh->getIndexedCount()));
	Q_ASSERT(mIndexBuffer->create());
}

void QDefaultProxyStaticMesh::recreatePipeline()
{
	if (mStaticMesh->getVertexCount() == 0) {
		return;
	}
	QVector<QRhiVertexInputBinding> inputBindings;
	inputBindings << QRhiVertexInputBinding{ sizeof(QStaticMeshComponent::Vertex) };
	QVector<QRhiVertexInputAttribute> attributeList;
	attributeList << QRhiVertexInputAttribute{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(QStaticMeshComponent::Vertex, position) };
	attributeList << QRhiVertexInputAttribute{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(QStaticMeshComponent::Vertex,normal) };
	attributeList << QRhiVertexInputAttribute{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(QStaticMeshComponent::Vertex,tangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(QStaticMeshComponent::Vertex,bitangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 4, QRhiVertexInputAttribute::Float3, offsetof(QStaticMeshComponent::Vertex,texCoord) };

	QString vertexShaderCode = R"(#version 440
	layout(location = 0) in vec3 inPosition;
	layout(location = 1) in vec3 inNormal;
	layout(location = 2) in vec3 inTangent;
	layout(location = 3) in vec3 inBitangent;
	layout(location = 4) in vec2 inUV;
	%1

	layout(location = 0) out vec2 vUV;

	out gl_PerVertex{
		vec4 gl_Position;
	};

	layout(std140,binding = 0) uniform buf{
		mat4 mvp;
	}ubuf;
	void main(){
		vUV = inUV;
		gl_Position = ubuf.mvp * %2 vec4(inPosition,1.0f);
	}
	)";

	if (bUsedByParticle) {
		inputBindings << QRhiVertexInputBinding{ sizeof(float) * 16 ,QRhiVertexInputBinding::PerInstance };
		attributeList << QRhiVertexInputAttribute{ 1, 5, QRhiVertexInputAttribute::Float4, 0,0 };
		attributeList << QRhiVertexInputAttribute{ 1, 6, QRhiVertexInputAttribute::Float4, 4 * sizeof(float),1 };
		attributeList << QRhiVertexInputAttribute{ 1, 7, QRhiVertexInputAttribute::Float4, 8 * sizeof(float),2 };
		attributeList << QRhiVertexInputAttribute{ 1, 8, QRhiVertexInputAttribute::Float4, 12 * sizeof(float),3 };
		vertexShaderCode = vertexShaderCode.arg("layout(location = 5) in mat4 insMat;")
			.arg("insMat*");
	}
	else {
		vertexShaderCode = vertexShaderCode.arg("")
			.arg("");
	}

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings(inputBindings.begin(), inputBindings.end());
	inputLayout.setAttributes(attributeList.begin(), attributeList.end());
	QShader vs = QSceneRenderer::createShaderFromCode(QShader::Stage::VertexStage, vertexShaderCode.toLocal8Bit());
	if (!vs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}
	const QRhiUniformProxy::UniformInfo& materialInfo = mStaticMesh->getMaterial()->getProxy()->getUniformInfo(1);

	QString defineCode = materialInfo.uniformDefineCode ;
	QString outputCode = mStaticMesh->getMaterial()->getShadingCode();

	if (mRenderer->debugEnabled()) {
		defineCode.prepend("layout (location = 1) out vec4 CompId;\n");
		outputCode.append(QString("CompId = %1;\n").arg(mStaticMesh->getCompIdVec4String()));
	}
	QString fragShaderCode = QString(R"(#version 440
	layout(location = 0) in vec2 vUV;
	layout(location = 0) out vec4 FragColor;
	%1
	void main(){
	    %2
	}
	)").arg(defineCode).arg(outputCode);

	QShader fs = QSceneRenderer::createShaderFromCode(QShader::Stage::FragmentStage, fragShaderCode.toLocal8Bit());
	if (!fs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}

	mPipeline.reset(RHI->newGraphicsPipeline());

	mPipeline->setVertexInputLayout(inputLayout);
	auto blends = mRenderer->getDefaultBlends();
	mPipeline->setTargetBlends(blends.begin(),blends.end());
	mPipeline->setTopology(mStaticMesh->getTopology());
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(mRenderer->getSampleCount());

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

	mPipeline->setRenderPassDescriptor(mRenderer->getRenderPassDescriptor().get());

	Q_ASSERT(mPipeline->create());
}

void QDefaultProxyStaticMesh::uploadResource(QRhiResourceUpdateBatch* batch)
{
	if (mStaticMesh->getBufferType() != QRhiBuffer::Dynamic) {
		batch->uploadStaticBuffer(mVertexBuffer.get(), mStaticMesh->getVertices().constData());
		batch->uploadStaticBuffer(mIndexBuffer.get(), mStaticMesh->getIndices().constData());
		mStaticMesh->bNeedUpdateIndex.receive();
		mStaticMesh->bNeedUpdateVertex.receive();
	}
}

void QDefaultProxyStaticMesh::updateResource(QRhiResourceUpdateBatch* batch) {
	if (mStaticMesh->bNeedUpdateVertex.receive()) {
		if (mStaticMesh->getBufferType() != QRhiBuffer::Dynamic || mVertexBuffer->size() != sizeof(QStaticMeshComponent::Vertex) * mStaticMesh->getVertices().size()) {
			mVertexBuffer.reset(RHI->newBuffer(mStaticMesh->getBufferType(), QRhiBuffer::VertexBuffer, sizeof(QStaticMeshComponent::Vertex) * mStaticMesh->getVertices().size()));
			Q_ASSERT(mVertexBuffer->create());
		}
		if (mStaticMesh->getBufferType() == QRhiBuffer::Dynamic)
			batch->updateDynamicBuffer(mVertexBuffer.get(), 0, sizeof(QStaticMeshComponent::Vertex) * mStaticMesh->getVertices().size(), mStaticMesh->getVertices().constData());
		else
			batch->uploadStaticBuffer(mVertexBuffer.get(), mStaticMesh->getVertices().constData());
	}
	if (mStaticMesh->bNeedUpdateIndex.receive()) {
		if (mStaticMesh->getBufferType() != QRhiBuffer::Dynamic || mIndexBuffer->size() != sizeof(QStaticMeshComponent::Index) * mStaticMesh->getIndices().size()) {
			mIndexBuffer.reset(RHI->newBuffer(mStaticMesh->getBufferType(), QRhiBuffer::IndexBuffer, sizeof(QStaticMeshComponent::Index) * mStaticMesh->getIndices().size()));
			Q_ASSERT(mIndexBuffer->create());
		}

		if (mStaticMesh->getBufferType() == QRhiBuffer::Dynamic)
			batch->updateDynamicBuffer(mIndexBuffer.get(), 0, sizeof(QStaticMeshComponent::Index) * mStaticMesh->getIndices().size(), mStaticMesh->getIndices().constData());
		else
			batch->uploadStaticBuffer(mIndexBuffer.get(), mStaticMesh->getVertices().constData());
	}

	QMatrix4x4 MVP = mRenderer->getVP() * mStaticMesh->calculateWorldMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(QMatrix4x4), MVP.constData());
}

void QDefaultProxyStaticMesh::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	if (!mPipeline)
		return;
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mStaticMesh->getIndexedCount());
}