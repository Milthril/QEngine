#include "SwapChainRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"

SwapChainRenderPass::SwapChainRenderPass()
{
}

void SwapChainRenderPass::setupSwapChain(QRhiSwapChain* swapchain)
{
	mSwapChain = swapchain;
}

void SwapChainRenderPass::compile()
{
	mTexturePainter.setupTexture(mInputTextures[InputTextureSlot::Color]);
	mTexturePainter.setupRenderPassDesc(mSwapChain->renderPassDescriptor());
	mTexturePainter.setupSampleCount(mSwapChain->sampleCount());
	mTexturePainter.compile();

	mDebugPainter.setupRenderPassDesc(mSwapChain->renderPassDescriptor());
	mDebugPainter.setupSampleCount(mSwapChain->sampleCount());
	mDebugPainter.setupWindow(mSwapChain->window());
	mDebugPainter.setupDebugTexture(mInputTextures[InputTextureSlot::DebugId]);
	mDebugPainter.compile();
}

void SwapChainRenderPass::execute(QRhiCommandBuffer* cmdBuffer)
{
	QRhiResourceUpdateBatch* batch = RHI->nextResourceUpdateBatch();
	mTexturePainter.resourceUpdate(batch);
	mDebugPainter.resourceUpdate(batch);
	cmdBuffer->resourceUpdate(batch);

	cmdBuffer->beginPass(mSwapChain->currentFrameRenderTarget(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f), { 1.0f, 0 });
	mTexturePainter.paint(cmdBuffer, mSwapChain->currentFrameRenderTarget());
	mDebugPainter.paint(cmdBuffer, mSwapChain->currentFrameRenderTarget());
	cmdBuffer->endPass();
}
