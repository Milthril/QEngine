#include "QDefaultProxyShape.h"
#include "private\qshaderbaker_p.h"
#include "Render\Scene\Component\QPrimitiveComponent.h"
#include "Render\Scene\Component\QShapeComponent.h"

QDefaultProxyShape::QDefaultProxyShape(QDefaultRenderer* renderer, std::shared_ptr<QShapeComponent> shape) :mShape(shape)
{
	auto mRhi = renderer->Rhi();
	QRhiBuffer* vbo = mRhi->newBuffer(shape->bufferType(), QRhiBuffer::VertexBuffer, sizeof(QPrimitiveComponent::Vertex) * shape->vertices().size());
	Q_ASSERT(vbo->create());
	mVertexBuffer.reset(vbo, [](QRhiBuffer* buffer) {
		buffer->destroy();
	});
	QRhiBuffer* ubo = mRhi->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(QMatrix4x4));
	Q_ASSERT(ubo->create());
	mUniformBuffer.reset(ubo, [](QRhiBuffer* buffer) {
		buffer->destroy();
	});

	const auto& indices = shape->indices();
	if (!indices.isEmpty()) {
		QRhiBuffer* ibo = mRhi->newBuffer(shape->bufferType(), QRhiBuffer::IndexBuffer, sizeof(QPrimitiveComponent::Index) * indices.size());
		Q_ASSERT(ibo->create());
		mIndexBuffer.reset(ibo, [](QRhiBuffer* buffer) {
			buffer->destroy();
		});
	}
	QRhiGraphicsPipeline* pipeline = mRhi->newGraphicsPipeline();
	QRhiGraphicsPipeline::TargetBlend blendState;
	pipeline->setTargetBlends({ blendState });
	pipeline->setTopology((QRhiGraphicsPipeline::Topology)shape->topology());
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
		{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,getPosition)},
		{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,normal)},
		{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,tangent)},
		{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,bitangent)},
		{ 0, 4, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,texCoord)},
		{ 0, 5, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,baseColor)},
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

void QDefaultProxyShape::updateResource(QRhiResourceUpdateBatch* batch, QMatrix4x4 VP)
{
	if (!m_vbufReady) {
		m_vbufReady = true;
		batch->uploadStaticBuffer(mVertexBuffer.get(), mShape->vertices().constData());
	}
	QMatrix4x4 MVP;
	MVP = VP * mShape->calculateModelMatrix();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(QMatrix4x4), MVP.constData());
}

void QDefaultProxyShape::draw(QRhiCommandBuffer* cmdBuffer)
{
	const QRhiCommandBuffer::VertexInput VertexInput(mVertexBuffer.get(), 0);
	cmdBuffer->setShaderResources();
	cmdBuffer->setVertexInput(0, 1, &VertexInput);
	cmdBuffer->draw(mShape->vertices().size());
}