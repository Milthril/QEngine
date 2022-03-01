#include "QFullSceneTexturePainter.h"

QFullSceneTexturePainter::QFullSceneTexturePainter(std::shared_ptr<QRhi> rhi)
	:mRhi(rhi)
{
}

void QFullSceneTexturePainter::initRhiResource(QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc, int sampleCount)
{
	mSampler.reset(mRhi->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();
	mPipeline.reset(mRhi->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipeline->setTargetBlends({ blendState });
	mPipeline->setSampleCount(sampleCount);
	QShader vs = QSceneRenderer::createShaderFromCode(QShader::VertexStage, R"(#version 450
layout (location = 0) out vec2 outUV;
out gl_PerVertex{
	vec4 gl_Position;
};
void main() {
	outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
}
)");

	QShader fs = QSceneRenderer::createShaderFromCode(QShader::FragmentStage, R"(#version 450
layout (binding = 0) uniform sampler2D samplerColor;
layout (location = 0) in vec2 inUV;
layout (location = 0) out vec4 outFragColor;
void main() {
	outFragColor = texture(samplerColor, inUV);
}
)");

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
	});
	QRhiVertexInputLayout inputLayout;

	mBindings.reset(mRhi->newShaderResourceBindings());
	mBindings->create();
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(renderPassDesc.get());
	mPipeline->create();
}

void QFullSceneTexturePainter::updateTexture(QRhiSPtr<QRhiTexture> texture)
{
	mTexture = texture;
	if (mBindings) {
		mBindings->destroy();
		mBindings->setBindings({
			QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mTexture.get(),mSampler.get())
		});
		mBindings->create();
	}
}

void QFullSceneTexturePainter::drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget)
{
	cmdBuffer->beginPass(renderTarget, QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);
	cmdBuffer->endPass();
}