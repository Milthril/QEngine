#include "QDefaultProxyText2D.h"
#include "private\qshaderbaker_p.h"
#include "Render\Scene\Component\QText2DComponent.h"

QDefaultProxyText2D::QDefaultProxyText2D(QDefaultRenderer* renderer, std::shared_ptr<QText2DComponent> shape) :mText2D(shape)
{
	auto mRhi = renderer->Rhi();
	static float vertices[] = {
		//	 pos			texCoord
			 1.0f, 1.0f,	0.0f,0.0f,
			 1.0f,-1.0f,	0.0f,0.0f,
			-1.0f, 1.0f,	0.0f,0.0f,
			-1.0f,-1.0f,	0.0f,0.0f
	};
	QRhiBuffer* vbo = mRhi->newBuffer(QRhiBuffer::Type::Immutable, QRhiBuffer::VertexBuffer, sizeof(vertices));
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

	void main(){
	    fragColor = texture();
	}
	)", QShader::Stage::FragmentStage);

	QShader fs = baker.bake();

	Q_ASSERT(fs.isValid());

	pipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
	});

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings({ sizeof(float) * 4 });

	inputLayout.setAttributes({
		{ 0, 0, QRhiVertexInputAttribute::Float2,0},
		{ 0, 1, QRhiVertexInputAttribute::Float2, sizeof(float) * 2},,
	});

	QRhiShaderResourceBindings* shaderBinding = mRhi->newShaderResourceBindings();
	shaderBinding->setBindings({
		QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,ubo)
	});
	shaderBinding->create();
	mShaderResourceBinding.reset(shaderBinding, [](QRhiShaderResourceBindings* binding) {
		binding->destroy();
	});

	pipeline->setVertexInputLayout(inputLayout);
	pipeline->setShaderResourceBindings(shaderBinding);
	pipeline->setRenderPassDescriptor(renderer->RenderPassDescriptor().get());
	Q_ASSERT(pipeline->create());

	mPipeline.reset(pipeline, [](QRhiGraphicsPipeline* pipline) {
		pipline->destroy();
	});
}

void QDefaultProxyText2D::updateResource(QRhiResourceUpdateBatch* batch, QMatrix4x4 VP)
{
	if (!m_vbufReady) {
		m_vbufReady = true;
		batch->uploadStaticBuffer(mVertexBuffer.get(), mText2D->vertices().constData());
	}
	QMatrix4x4 MVP;
	MVP = VP * mText2D->calculateModelMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(QMatrix4x4), MVP.constData());
}

void QDefaultProxyText2D::draw(QRhiCommandBuffer* cmdBuffer)
{
	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	cmdBuffer->setShaderResources();
	cmdBuffer->setVertexInput(0, 1, &VertexInput);
	cmdBuffer->draw(mText2D->vertices().size());
}