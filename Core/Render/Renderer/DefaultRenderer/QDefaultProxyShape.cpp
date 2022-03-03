#include "QDefaultProxyShape.h"
#include "private\qshaderbaker_p.h"
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

	if (!mShape->getIndices().isEmpty()) {
		mIndexBuffer.reset(mRhi->newBuffer(mShape->getBufferType(), QRhiBuffer::IndexBuffer, sizeof(QPrimitiveComponent::Index) * mShape->getIndices().size()));
		Q_ASSERT(mIndexBuffer->create());
	}

	mSampler.reset(mRhi->newSampler(QRhiSampler::Linear, QRhiSampler::Linear, QRhiSampler::None,
				   QRhiSampler::ClampToEdge, QRhiSampler::ClampToEdge));
	mSampler->create();

	QRhiGraphicsPipeline* pipeline = mRhi->newGraphicsPipeline();
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	pipeline->setTargetBlends({ blendState });
	pipeline->setTopology(mShape->getTopology());
	pipeline->setDepthTest(true);
	pipeline->setDepthWrite(true);
	pipeline->setSampleCount(mRenderer->getSampleCount());
	QShaderBaker baker;
	baker.setGeneratedShaderVariants({ QShader::StandardShader });
	baker.setGeneratedShaders({
		QShaderBaker::GeneratedShader{QShader::Source::SpirvShader,QShaderVersion(100)},
		QShaderBaker::GeneratedShader{QShader::Source::GlslShader,QShaderVersion(120)},
		QShaderBaker::GeneratedShader{QShader::Source::MslShader,QShaderVersion(12)},
		QShaderBaker::GeneratedShader{QShader::Source::HlslShader,QShaderVersion(50)},
	});

	baker.setSourceString(R"(#version 440
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
	)", QShader::Stage::VertexStage);

	QShader vs = baker.bake();

	baker.setSourceString(R"(#version 440
	layout(location = 0) in vec4 vBaseColor;
	layout(location = 0) out vec4 fragColor;
	void main(){
	    fragColor = vBaseColor;
	}
	)", QShader::Stage::FragmentStage);

	QShader fs = baker.bake();

	Q_ASSERT(fs.isValid());

	pipeline->setShaderStages({
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
	pipeline->setVertexInputLayout(inputLayout);

	mShaderResourceBinding.reset(mRhi->newShaderResourceBindings());
	mShaderResourceBinding->setBindings({
		QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get())
	});
	mShaderResourceBinding->create();
	pipeline->setShaderResourceBindings(mShaderResourceBinding.get());

	pipeline->setRenderPassDescriptor(mRenderer->getRenderPassDescriptor().get());

	Q_ASSERT(pipeline->create());
	mPipeline.reset(pipeline, [](QRhiGraphicsPipeline* pipline) {
		pipline->destroy();
	});
}

void QDefaultProxyShape::uploadResource(QRhiResourceUpdateBatch* batch)
{
	if (mShape->getBufferType() != QRhiBuffer::Dynamic)
		batch->uploadStaticBuffer(mVertexBuffer.get(), mShape->getVertices().constData());
	if (mIndexBuffer) {
		if (mShape->getBufferType() != QRhiBuffer::Dynamic)
			batch->uploadStaticBuffer(mIndexBuffer.get(), mShape->getVertices().constData());
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
		mShaderResourceBinding->setBindings({
			QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get()),
		});
		mShaderResourceBinding->create();
		mTexture.reset(nullptr);
	}

	QMatrix4x4 MVP = mRenderer->getVP() * mShape->calculateModelMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(QMatrix4x4), MVP.constData());
}

void QDefaultProxyShape::draw(QRhiCommandBuffer* cmdBuffer) {
	cmdBuffer->setShaderResources();
	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	if (mShape->getIndices().isEmpty()) {
		cmdBuffer->setVertexInput(0, 1, &VertexInput);
		cmdBuffer->draw(mShape->getVertices().size());
	}
	else {
		cmdBuffer->setVertexInput(0, 1, &VertexInput, mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
		cmdBuffer->drawIndexed(mShape->getIndices().size());
	}
}