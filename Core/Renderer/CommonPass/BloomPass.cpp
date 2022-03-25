#include "BloomPass.h"
#include "Renderer\QSceneRenderer.h"
#include "QEngine.h"

BloomPass::BloomPass()
	: mPixelSelectPass(R"(
void main() {
	vec4 color = texture(uTexture, vUV);
	float value = max(max(color.r,color.g),color.b);
	outFragColor = (1-step(value,1.0f)) * color;
}
)")
{
}

void BloomPass::makeBloom(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> inputTexture, QRhiRenderTarget* renderTarget) {
	mBlurPass.createOrResize(inputTexture->pixelSize() / 2);
	mPixelSelectPass.drawCommand(cmdBuffer, inputTexture, mBlurPass.getInputRenderTaget().get());
	mBlurPass.makeBlur(cmdBuffer, inputTexture);
	mMeragePainter.updateTexture(inputTexture, mBlurPass.getOutputTexture(), renderTarget);
}

void BloomPass::drawInPass(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget)
{
	mMeragePainter.drawInPass(cmdBuffer, renderTarget);
}

QBloomMeragePainter::QBloomMeragePainter()
{
}

void QBloomMeragePainter::initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom)
{
	mSrcTexture = src;
	mBloomTexture = bloom;
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
	QShader vs = QSceneRenderer::createShaderFromCode(QShader::VertexStage, R"(#version 450
layout (location = 0) out vec2 vUV;
out gl_PerVertex{
	vec4 gl_Position;
};
void main() {
	vUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(vUV * 2.0f - 1.0f, 0.0f, 1.0f);
}
)");

	QShader fs = QSceneRenderer::createShaderFromCode(QShader::FragmentStage, R"(#version 450
layout (binding = 0) uniform sampler2D uSrcTexture;
layout (binding = 1) uniform sampler2D uBloomTexture;
layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 outFragColor;
void main() {
	vec4 srcColor = texture(uSrcTexture, vUV);
	vec4 bloomColor = texture(uBloomTexture, vUV);

	float gamma = 1.0f;
	vec3 mapped = vec3(1.0)-exp(-(srcColor.rgb+bloomColor.rgb));
	mapped = pow(mapped,vec3(1.0f/gamma));
	outFragColor = vec4(max(mapped,srcColor.rgb),1.0);
}
)");

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							   });

	QRhiVertexInputLayout inputLayout;

	mBindings.reset(RHI->newShaderResourceBindings());
	mBindings->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mSrcTexture.get(),mSampler.get()),
		QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mBloomTexture.get(),mSampler.get())
						   });

	mBindings->create();
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(renderPassDesc);
	mPipeline->create();
}

void QBloomMeragePainter::updateTexture(QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom, QRhiRenderTarget* renderTarget)
{
	if (src != mSrcTexture) {
		if (!mSrcTexture)
			initRhiResource(renderTarget->renderPassDescriptor(), renderTarget, src, bloom);
		else {
			mSrcTexture = src;
			mBloomTexture = bloom;
			if (mBindings) {
				mBindings->destroy();
				mBindings->setBindings({
					QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mSrcTexture.get(),mSampler.get()),
					QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mBloomTexture.get(),mSampler.get())
									   });
				mBindings->create();
			}
		}
	}
}

void QBloomMeragePainter::drawInPass(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget)
{
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, renderTarget->pixelSize().width(), renderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);
}