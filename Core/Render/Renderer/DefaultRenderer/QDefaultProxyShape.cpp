#include "QDefaultProxyShape.h"
#include "Render\Scene\Component\QPrimitiveComponent.h"
#include "Render\Scene\Component\QShapeComponent.h"

QDefaultProxyShape::QDefaultProxyShape(std::shared_ptr<QShapeComponent> shape)
	:mShape(shape)
{
}

void QDefaultProxyShape::recreateResource()
{
	mUniformBuffer.reset(mRhi->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(QMatrix4x4)));
	Q_ASSERT(mUniformBuffer->create());

	mVertexBuffer.reset(mRhi->newBuffer(mShape->getBufferType(), QRhiBuffer::VertexBuffer, sizeof(QPrimitiveComponent::Vertex) * mShape->getVertices().size()));
	Q_ASSERT(mVertexBuffer->create());

	if (mShape->getIndices().isEmpty()) {
		mShape->autoFillIndices();
	}
	mIndexBuffer.reset(mRhi->newBuffer(mShape->getBufferType(), QRhiBuffer::IndexBuffer, sizeof(QPrimitiveComponent::Index) * mShape->getIndices().size()));
	Q_ASSERT(mIndexBuffer->create());

	mMaterialProxy.reset(new QMaterialProxy(mShape->getMaterial().get(), mRhi));
	mMaterialProxy->recreateResource();
}

void QDefaultProxyShape::recreatePipeline(PipelineUsageFlags flags /*= PipelineUsageFlag::Normal*/)
{
	mPipeline.reset(mRhi->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipeline->setTargetBlends({ blendState });
	mPipeline->setTopology(mShape->getTopology());
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(mRenderer->getSampleCount());

	QVector<QRhiVertexInputBinding> inputBindings;
	inputBindings << QRhiVertexInputBinding{ sizeof(QPrimitiveComponent::Vertex) };

	QVector<QRhiVertexInputAttribute> attributeList;

	attributeList << QRhiVertexInputAttribute{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex, position) };
	attributeList << QRhiVertexInputAttribute{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,normal) };
	attributeList << QRhiVertexInputAttribute{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,tangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,bitangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 4, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,texCoord) };

	QString vertexShaderCode = R"(#version 440
	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 normal;
	layout(location = 2) in vec3 tangent;
	layout(location = 3) in vec3 bitangent;
	layout(location = 4) in vec2 texCoord;
	%1

	out gl_PerVertex{
		vec4 gl_Position;
	};

	layout(std140,binding = 0) uniform buf{
		mat4 mvp;
	}ubuf;

	void main(){
		gl_Position = ubuf.mvp * %2 vec4(position,1.0f);
	}
	)";

	if (flags.testFlag(QRhiProxy::Instancing)) {
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
	mPipeline->setVertexInputLayout(inputLayout);

	QShader vs = QSceneRenderer::createShaderFromCode(QShader::Stage::VertexStage, vertexShaderCode.toLocal8Bit());

	const QMaterialProxy::MaterialShaderInfo& materialInfo = mMaterialProxy->getMaterialShaderInfo(1);;
	QString fragShaderCode = QString(R"(#version 440
	layout(location = 0) out vec4 FragColor;
	%1
	void main(){
	    %2
	}
	)").arg(materialInfo.uniformCode, materialInfo.shadingCode);
	QShader fs = QSceneRenderer::createShaderFromCode(QShader::Stage::FragmentStage, fragShaderCode.toLocal8Bit());
	Q_ASSERT(fs.isValid());

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
	});

	mShaderResourceBindings.reset(mRhi->newShaderResourceBindings());

	QVector<QRhiShaderResourceBinding> shaderBindings;
	shaderBindings << QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, mUniformBuffer.get());
	shaderBindings << materialInfo.bindings;
	mShaderResourceBindings->setBindings(shaderBindings.begin(), shaderBindings.end());

	mShaderResourceBindings->create();

	mPipeline->setShaderResourceBindings(mShaderResourceBindings.get());

	mPipeline->setRenderPassDescriptor(mRenderer->getRenderPassDescriptor().get());

	Q_ASSERT(mPipeline->create());
}

void QDefaultProxyShape::uploadResource(QRhiResourceUpdateBatch* batch)
{
	if (mShape->getBufferType() != QRhiBuffer::Dynamic) {
		batch->uploadStaticBuffer(mVertexBuffer.get(), mShape->getVertices().constData());
		batch->uploadStaticBuffer(mIndexBuffer.get(), mShape->getIndices().constData());
	}
}

void QDefaultProxyShape::updateResource(QRhiResourceUpdateBatch* batch) {
	if (mShape->bNeedUpdateVertex) {
		mShape->bNeedUpdateVertex = false;
		mVertexBuffer.reset(mRhi->newBuffer(mShape->getBufferType(), QRhiBuffer::VertexBuffer, sizeof(QPrimitiveComponent::Vertex) * mShape->getVertices().size()));
		Q_ASSERT(mVertexBuffer->create());
		if (mShape->getBufferType() == QRhiBuffer::Dynamic)
			batch->updateDynamicBuffer(mVertexBuffer.get(), 0, sizeof(QPrimitiveComponent::Vertex) * mShape->getVertices().size(), mShape->getVertices().constData());
		else
			batch->uploadStaticBuffer(mVertexBuffer.get(), mShape->getVertices().constData());
	}
	if (mShape->bNeedUpdateIndex) {
		mShape->bNeedUpdateIndex = false;
		mIndexBuffer.reset(mRhi->newBuffer(mShape->getBufferType(), QRhiBuffer::IndexBuffer, sizeof(QPrimitiveComponent::Index) * mShape->getIndices().size()));
		Q_ASSERT(mIndexBuffer->create());

		if (mShape->getBufferType() == QRhiBuffer::Dynamic)
			batch->updateDynamicBuffer(mIndexBuffer.get(), 0, sizeof(QPrimitiveComponent::Index) * mShape->getIndices().size(), mShape->getIndices().constData());
		else
			batch->uploadStaticBuffer(mIndexBuffer.get(), mShape->getVertices().constData());
	}
	mMaterialProxy->updateResource(batch);
	//if (mShape->bNeedUpdateTexture) {
	//	mShape->bNeedUpdateTexture = false;
	//	QImage image = std::move(mShape->getTexture());
	//	if (!image.isNull()) {
	//		QRhiTexture* newTexture = mRhi->newTexture(QRhiTexture::RGBA8, image.size(), 1, QRhiTexture::UsedAsTransferSource);
	//		newTexture->create();
	//		mTexture.reset(newTexture, [](QRhiTexture* texture) {
	//			texture->destroy();
	//		});
	//		mShaderResourceBinding->setBindings({
	//			QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get()),
	//			QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mTexture.get(),mSampler.get())
	//		});
	//		mShaderResourceBinding->create();
	//	}
	//}
	if (mShape->getTexture().isNull() && mTexture) {
		mShaderResourceBindings->setBindings({
			QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get()),
		});
		mShaderResourceBindings->create();
		mTexture.reset(nullptr);
	}

	QMatrix4x4 MVP = mRenderer->getVP() * mShape->calculateModelMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(QMatrix4x4), MVP.constData());
}

void QDefaultProxyShape::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mShape->getIndexedCount());
}