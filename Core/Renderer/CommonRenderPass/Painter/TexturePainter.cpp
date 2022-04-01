#include "TexturePainter.h"
#include "Renderer\ISceneRenderer.h"
#include "QEngine.h"

TexturePainter::TexturePainter()
{
}

void TexturePainter::setupRenderTarget(QRhiRenderTarget* renderTarget)
{
	mRenderTarget = renderTarget;
}

void TexturePainter::setupCmdBuffer(QRhiCommandBuffer* cmdBuffer)
{
	mCmdBuffer = cmdBuffer;
}

void TexturePainter::setupTexture(QRhiTexture* texture)
{
	mTexture = texture;
	if (mBindings) {
		mBindings->destroy();
		mBindings->setBindings({
			QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mTexture,mSampler.get())
							   });
		mBindings->create();
	}
}

void TexturePainter::compile()
{
	mSampler.reset(RHI->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();
	mPipeline.reset(RHI->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipeline->setTargetBlends({ blendState });
	mPipeline->setSampleCount(mRenderTarget->sampleCount());
	QShader vs = ISceneRenderer::createShaderFromCode(QShader::VertexStage, R"(#version 450
layout (location = 0) out vec2 vUV;
out gl_PerVertex{
	vec4 gl_Position;
};
void main() {
	vUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(vUV * 2.0f - 1.0f, 0.0f, 1.0f);
}
)");

	QShader fs = ISceneRenderer::createShaderFromCode(QShader::FragmentStage, R"(#version 450
layout (binding = 0) uniform sampler2D samplerColor;
layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 outFragColor;
void main() {
	outFragColor = texture(samplerColor, vUV);
}
)");

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							   });
	QRhiVertexInputLayout inputLayout;

	mBindings.reset(RHI->newShaderResourceBindings());
	mBindings->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mTexture,mSampler.get())
						   });
	mBindings->create();
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(mRenderTarget->renderPassDescriptor());
	mPipeline->create();
}

void TexturePainter::execute()
{
	mCmdBuffer->beginPass(mRenderTarget, QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	mCmdBuffer->setGraphicsPipeline(mPipeline.get());
	mCmdBuffer->setViewport(QRhiViewport(0, 0, mRenderTarget->pixelSize().width(), mRenderTarget->pixelSize().height()));
	mCmdBuffer->setShaderResources(mBindings.get());
	mCmdBuffer->draw(4);
	mCmdBuffer->endPass();
}