#include "QDefaultProxyText2D.h"
#include "private\qshaderbaker_p.h"
#include "Render\Scene\Component\QText2DComponent.h"
#include "QFontMetrics"
#include "QPainterPath"
#include "QPainter"

static float getVertices[] = {
	//	 pos			texCoord
		 1.0f, 1.0f,	1.0f,0.0f,
		 1.0f,-1.0f,	1.0f,1.0f,
		-1.0f, 1.0f,	0.0f,0.0f,
		-1.0f,-1.0f,	0.0f,1.0f
};

QDefaultProxyText2D::QDefaultProxyText2D(std::shared_ptr<QText2DComponent> shape)
	:mText2D(shape)
{
}

QImage QDefaultProxyText2D::createTextImage(const QString& text, const QFont& font, const QBrush& brush, const int& spacing /*= 0*/, Qt::Orientation o /*= Qt::Orientation::Horizontal*/)
{
	if (text.isEmpty())
		return {};
	QFontMetrics fontMetrics(font);
	QSize size;
	QPainterPath fontPath;
	if (o == Qt::Orientation::Horizontal) {
		size = { 0,fontMetrics.height() };
		for (int i = 0; i < text.size(); i++) {
			fontPath.addText(size.width(), fontMetrics.ascent(), font, text[i]);
			size.setWidth(size.width() + spacing + fontMetrics.horizontalAdvance(text[i]));
		}
	}
	else {
		size = { 0,fontMetrics.ascent() };
		for (int i = 0; i < text.size(); i++) {
			fontPath.addText(0, size.height(), font, text[i]);
			size.setWidth(qMax(size.width(), fontMetrics.horizontalAdvance(text[i])));
			size.setHeight(size.height() + spacing + fontMetrics.height());;
		}
	}
	QImage image(size, QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setFont(font);
	painter.fillPath(fontPath, brush);
	return image;
}

void QDefaultProxyText2D::recreateResource()
{
	QRhiBuffer* vbo = mRhi->newBuffer(QRhiBuffer::Type::Immutable, QRhiBuffer::VertexBuffer, sizeof(getVertices));
	Q_ASSERT(vbo->create());
	mVertexBuffer.reset(vbo, [](QRhiBuffer* buffer) {
		buffer->destroy();
	});

	QRhiBuffer* ubo = mRhi->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(QMatrix4x4));
	Q_ASSERT(ubo->create());
	mUniformBuffer.reset(ubo, [](QRhiBuffer* buffer) {
		buffer->destroy();
	});

	QRhiGraphicsPipeline* pipeline = mRhi->newGraphicsPipeline();
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	pipeline->setDepthTest(true);
	pipeline->setDepthWrite(true);
	pipeline->setTargetBlends({ blendState });
	pipeline->setTopology(QRhiGraphicsPipeline::Topology::TriangleStrip);
	QShaderBaker baker;
	baker.setGeneratedShaderVariants({ QShader::StandardShader });
	baker.setGeneratedShaders({
		QShaderBaker::GeneratedShader{QShader::Source::SpirvShader,QShaderVersion(100)},
		QShaderBaker::GeneratedShader{QShader::Source::GlslShader,QShaderVersion(120)},
		QShaderBaker::GeneratedShader{QShader::Source::MslShader,QShaderVersion(12)},
		QShaderBaker::GeneratedShader{QShader::Source::HlslShader,QShaderVersion(50)},
	});

	baker.setSourceString(R"(#version 440
	layout(location = 0) in vec2 position;
	layout(location = 1) in vec2 texCoords;
	layout(location = 0) out vec2 vTexCoords;

	out gl_PerVertex{
		vec4 gl_Position;
	};

	layout(std140,binding = 0) uniform buf{
		mat4 mvp;
	}ubuf;

	void main(){
		vTexCoords = texCoords;
		gl_Position = ubuf.mvp * vec4(position,0.0f,1.0f);
	}
	)", QShader::Stage::VertexStage);

	QShader vs = baker.bake();

	baker.setSourceString(R"(#version 440
	layout(location = 0) in vec2 vTexCoords;
	layout(location = 0) out vec4 fragColor;
	layout(binding = 1) uniform sampler2D tex;

	void main(){
	    fragColor = texture(tex,vTexCoords);
	}
	)", QShader::Stage::FragmentStage);

	QShader fs = baker.bake();
	qDebug() << baker.errorMessage();
	Q_ASSERT(fs.isValid());

	pipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
	});

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings({ sizeof(float) * 4 });

	inputLayout.setAttributes({
		{ 0, 0, QRhiVertexInputAttribute::Float2,0},
		{ 0, 1, QRhiVertexInputAttribute::Float2, sizeof(float) * 2},
	});

	QRhiSampler* sampler = mRhi->newSampler(QRhiSampler::Linear, QRhiSampler::Linear, QRhiSampler::None,
								QRhiSampler::ClampToEdge, QRhiSampler::ClampToEdge);
	sampler->create();
	mSampler.reset(sampler, [](QRhiSampler* sampler) {
		sampler->destroy();
	});

	mTextImage = std::move(createTextImage(mText2D->getText(), QFont(), Qt::red));
	QRhiTexture* texture = mRhi->newTexture(QRhiTexture::RGBA8, mTextImage.size(), 1, QRhiTexture::UsedAsTransferSource);
	texture->create();
	mTexture.reset(texture, [](QRhiTexture* texture) {
		texture->destroy();
	});

	QRhiShaderResourceBindings* shaderBinding = mRhi->newShaderResourceBindings();
	shaderBinding->setBindings({
		QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get()),
		QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mTexture.get(),mSampler.get())
	});
	shaderBinding->create();
	mShaderResourceBinding.reset(shaderBinding, [](QRhiShaderResourceBindings* binding) {
		binding->destroy();
	});

	pipeline->setVertexInputLayout(inputLayout);
	pipeline->setShaderResourceBindings(shaderBinding);
	pipeline->setRenderPassDescriptor(mRenderer->RenderPassDescriptor().get());
	Q_ASSERT(pipeline->create());

	mPipeline.reset(pipeline, [](QRhiGraphicsPipeline* pipline) {
		pipline->destroy();
	});
}

void QDefaultProxyText2D::updateResource(QRhiResourceUpdateBatch* batch)
{
	if (!mInit) {
		mInit = true;
		batch->uploadStaticBuffer(mVertexBuffer.get(), getVertices);
	}
	if (mText2D->hasNewText) {
		mText2D->hasNewText = false;
		if (mTextImage.isNull()) {
			mTextImage = std::move(createTextImage(mText2D->getText(), QFont(), Qt::red));
		}
		if (!mTexture || mTexture->pixelSize() != mTextImage.size()) {
			QRhiTexture* newTexture = mRhi->newTexture(QRhiTexture::RGBA8, mTextImage.size(), 1, QRhiTexture::UsedAsTransferSource);
			newTexture->create();
			mTexture.reset(newTexture, [](QRhiTexture* texture) {
				texture->destroy();
			});
			mShaderResourceBinding->setBindings({
				QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get()),
				QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mTexture.get(),mSampler.get())
			});
			mShaderResourceBinding->create();
		}
		batch->uploadTexture(mTexture.get(), mTextImage);
		mTextImage = {};
	}
	QMatrix4x4 MVP = mRenderer->getVP() * mText2D->calculateModelMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(QMatrix4x4), MVP.constData());
}

void QDefaultProxyText2D::draw(QRhiCommandBuffer* cmdBuffer)
{
	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	cmdBuffer->setShaderResources();
	cmdBuffer->setVertexInput(0, 1, &VertexInput);
	cmdBuffer->draw(4);
}