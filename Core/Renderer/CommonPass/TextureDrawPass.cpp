#include "TextureDrawPass.h"
#include "Renderer\ISceneRenderer.h"
#include "QEngine.h"

TextureDrawPass::TextureDrawPass()
{
}

void TextureDrawPass::initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> texture)
{
	mTexture = texture;
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
	mPipeline->setSampleCount(renderTarget->sampleCount());
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
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mTexture.get(),mSampler.get())
						   });
	mBindings->create();
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(renderPassDesc);
	mPipeline->create();
}

void TextureDrawPass::updateTexture(QRhiSPtr<QRhiTexture> texture)
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

void TextureDrawPass::drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> texture, QRhiRenderTarget* renderTarget)
{
	auto it = renderTarget->renderPassDescriptor();
	if (texture != mTexture) {
		if (!mTexture)
			initRhiResource(renderTarget->renderPassDescriptor(), renderTarget, texture);
		else
			updateTexture(texture);
	}
	cmdBuffer->beginPass(renderTarget, QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, renderTarget->pixelSize().width(), renderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);
	cmdBuffer->endPass();
}