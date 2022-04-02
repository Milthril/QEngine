#include "BloomRenderPass.h"
#include "Renderer\ISceneRenderer.h"
#include "QEngine.h"

BloomRenderPass::BloomRenderPass()
	: mPixelSelectPainter()
{
}

void BloomRenderPass::setupInputTexture(QRhiTexture* texture) {
	mInputTexture = texture;
}

void BloomRenderPass::compile() {
	mRT.colorAttachment.reset(RHI->newTexture(QRhiTexture::RGBA32F, mInputTexture->pixelSize(), 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.colorAttachment->create();
	mRT.renderTarget.reset(RHI->newTextureRenderTarget({ mRT.colorAttachment.get() }));
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->create();

	mPixelSelectPainter.setupInputTexture(mInputTexture);
	mPixelSelectPainter.setupSelectCode(R"(
void main() {
	vec4 color = texture(uTexture, vUV);
	float value = max(max(color.r,color.g),color.b);
	outFragColor = (1-step(value,1.0f)) * color;
}
)");
	mPixelSelectPainter.compile();

	mBlurPainter.setupInputTexture(mPixelSelectPainter.getOutputTexture());
	mBlurPainter.compile();

	mMeragePainter.setupBloomTexutre(mBlurPainter.getOutputTexture());
	mMeragePainter.setupSrcTexutre(mInputTexture);
	mMeragePainter.setupRenderTarget(mRT.renderTarget.get());		
	mMeragePainter.compile();
}

void BloomRenderPass::execute() {
	QRhiCommandBuffer* cmdBuffer;
	if (RHI->beginOffscreenFrame(&cmdBuffer) == QRhi::FrameOpSuccess) {
		mPixelSelectPainter.paint(cmdBuffer);
		mBlurPainter.paint(cmdBuffer);
		mMeragePainter.paint(cmdBuffer);
		//QRhiResourceUpdateBatch* u = RHI->nextResourceUpdateBatch();
		//QRhiReadbackDescription desc(mPixelSelectPainter.getOutputTexture());
		//QRhiReadbackResult* ret =new QRhiReadbackResult;
		//ret->completed = [ret]() {
		//	QImage image((uchar*)ret->data.data(), ret->pixelSize.width(), ret->pixelSize.height(), QImage::Format_RGBA8888);
		//	image.save("tem.png");
		//};

		//u->readBackTexture(desc, ret);
		//cmdBuffer->resourceUpdate(u);
		RHI->endOffscreenFrame();
	}
}

QRhiTexture* BloomRenderPass::getOutputTexture() {
	return mRT.colorAttachment.get();
}

QRhiTexture* BloomRenderPass::getSelectTexture() {
	return mPixelSelectPainter.getOutputTexture();
}

QRhiTexture* BloomRenderPass::getBlurTexture() {
	return mBlurPainter.getOutputTexture();
}

QBloomMeragePainter::QBloomMeragePainter(){}

void QBloomMeragePainter::setupRenderTarget(QRhiRenderTarget* renderTarget) {
	mRenderTarget = renderTarget;
}

void QBloomMeragePainter::setupSrcTexutre(QRhiTexture* texture) {
	mSrcTexture = texture;
}

void QBloomMeragePainter::setupBloomTexutre(QRhiTexture* texture) {
	mBloomTexture = texture;
}

void QBloomMeragePainter::compile() {
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
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mSrcTexture,mSampler.get()),
		QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mBloomTexture,mSampler.get())
						   });

	mBindings->create();
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(mRenderTarget->renderPassDescriptor());
	mPipeline->create();
}

void QBloomMeragePainter::paint(QRhiCommandBuffer* cmdBuffer) {
	cmdBuffer->beginPass(mRenderTarget, QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, mRenderTarget->pixelSize().width(),mRenderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);
	cmdBuffer->endPass();
}
