#include "QDefaultProxyShape.h"
#include "Render\Scene\Component\QPrimitiveComponent.h"
#include "Render\Scene\Component\QShapeComponent.h"

QDefaultProxyShape::QDefaultProxyShape(std::shared_ptr<QShapeComponent> shape)
	:mParticle(shape)
{
}

void QDefaultProxyShape::recreateResource()
{
	mUniformBuffer.reset(mRhi->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(QMatrix4x4)));
	Q_ASSERT(mUniformBuffer->create());

	mVertexBuffer.reset(mRhi->newBuffer(mParticle->getBufferType(), QRhiBuffer::VertexBuffer, sizeof(QPrimitiveComponent::Vertex) * mParticle->getVertices().size()));
	Q_ASSERT(mVertexBuffer->create());

	if (!mParticle->getIndices().isEmpty()) {
		mIndexBuffer.reset(mRhi->newBuffer(mParticle->getBufferType(), QRhiBuffer::IndexBuffer, sizeof(QPrimitiveComponent::Index) * mParticle->getIndices().size()));
		Q_ASSERT(mIndexBuffer->create());
	}

	mSampler.reset(mRhi->newSampler(QRhiSampler::Linear, QRhiSampler::Linear, QRhiSampler::None,
				   QRhiSampler::ClampToEdge, QRhiSampler::ClampToEdge));
	mSampler->create();

	mPipeline.reset(mRhi->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipeline->setTargetBlends({ blendState });
	mPipeline->setTopology(mParticle->getTopology());
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(mRenderer->getSampleCount());

	QShader vs = QSceneRenderer::createShaderFromCode(QShader::Stage::VertexStage, R"(#version 440
	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 normal;
	layout(location = 2) in vec3 tangent;
	layout(location = 3) in vec3 bitangent;
	layout(location = 4) in vec2 texCoord;
	layout(location = 5) in vec4 baseColor;

	layout(location = 0) out vec4 vBaseColor;

	out gl_PerVertex{
		vec4 gl_Position;
	};

	layout(std140,binding = 0) uniform buf{
		mat4 mvp;
	}ubuf;

	void main(){
		vBaseColor = baseColor;
		gl_Position = ubuf.mvp * vec4(position,1.0f);
	}
	)");

	QShader fs = QSceneRenderer::createShaderFromCode(QShader::Stage::FragmentStage, R"(#version 440
	layout(location = 0) in vec4 vBaseColor;
	layout(location = 0) out vec4 fragColor;
	void main(){
	    fragColor = vBaseColor;
	}
	)");

	Q_ASSERT(fs.isValid());

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
	});

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings({ sizeof(QPrimitiveComponent::Vertex) });
	inputLayout.setAttributes({
		{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,position)},
		{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,normal)},
		{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,tangent)},
		{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,bitangent)},
		{ 0, 4, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,texCoord)},
		{ 0, 5, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,baseColor)},
	});
	mPipeline->setVertexInputLayout(inputLayout);

	mShaderResourceBindings.reset(mRhi->newShaderResourceBindings());
	mShaderResourceBindings->setBindings({
		QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get())
	});
	mShaderResourceBindings->create();
	mPipeline->setShaderResourceBindings(mShaderResourceBindings.get());

	mPipeline->setRenderPassDescriptor(mRenderer->getRenderPassDescriptor().get());

	Q_ASSERT(mPipeline->create());
}

void QDefaultProxyShape::uploadResource(QRhiResourceUpdateBatch* batch)
{
	if (mParticle->getBufferType() != QRhiBuffer::Dynamic)
		batch->uploadStaticBuffer(mVertexBuffer.get(), mParticle->getVertices().constData());
	if (mIndexBuffer) {
		if (mParticle->getBufferType() != QRhiBuffer::Dynamic)
			batch->uploadStaticBuffer(mIndexBuffer.get(), mParticle->getVertices().constData());
	}
}

void QDefaultProxyShape::updateResource(QRhiResourceUpdateBatch* batch) {
	if (mParticle->bNeedUpdateVertex) {
		mParticle->bNeedUpdateVertex = false;
		mVertexBuffer.reset(mRhi->newBuffer(mParticle->getBufferType(), QRhiBuffer::VertexBuffer, sizeof(QPrimitiveComponent::Vertex) * mParticle->getVertices().size()));
		Q_ASSERT(mVertexBuffer->create());
		if (mParticle->getBufferType() == QRhiBuffer::Dynamic)
			batch->updateDynamicBuffer(mVertexBuffer.get(), 0, sizeof(QPrimitiveComponent::Vertex) * mParticle->getVertices().size(), mParticle->getVertices().constData());
		else
			batch->uploadStaticBuffer(mVertexBuffer.get(), mParticle->getVertices().constData());
	}
	if (mParticle->bNeedUpdateIndex) {
		mParticle->bNeedUpdateIndex = false;
		mIndexBuffer.reset(mRhi->newBuffer(mParticle->getBufferType(), QRhiBuffer::IndexBuffer, sizeof(QPrimitiveComponent::Index) * mParticle->getIndices().size()));
		Q_ASSERT(mIndexBuffer->create());

		if (mParticle->getBufferType() == QRhiBuffer::Dynamic)
			batch->updateDynamicBuffer(mIndexBuffer.get(), 0, sizeof(QPrimitiveComponent::Index) * mParticle->getIndices().size(), mParticle->getIndices().constData());
		else
			batch->uploadStaticBuffer(mIndexBuffer.get(), mParticle->getVertices().constData());
	}
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
	if (mParticle->getTexture().isNull() && mTexture) {
		mShaderResourceBindings->setBindings({
			QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get()),
		});
		mShaderResourceBindings->create();
		mTexture.reset(nullptr);
	}

	QMatrix4x4 MVP = mRenderer->getVP() * mParticle->calculateModelMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(QMatrix4x4), MVP.constData());
}

void QDefaultProxyShape::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();

	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	if (mParticle->getIndices().isEmpty()) {
		cmdBuffer->setVertexInput(0, 1, &VertexInput);
		cmdBuffer->draw(mParticle->getVertices().size());
	}
	else {
		cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
		cmdBuffer->drawIndexed(mParticle->getIndices().size());
	}
}