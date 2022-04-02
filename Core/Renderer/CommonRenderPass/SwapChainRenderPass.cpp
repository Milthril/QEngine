#include "SwapChainRenderPass.h"
#include "QEngine.h"
#include "Painter\DebugPainter.h"

SwapChainRenderPass::SwapChainRenderPass()
{
}

void SwapChainRenderPass::setupTexture(QRhiTexture* texture)
{
	mTexture = texture;
}

void SwapChainRenderPass::setupSwapChain(QRhiSwapChain* swapchain)
{
	mSwapChain = swapchain;
}

void SwapChainRenderPass::compile()
{
	mTexturePainter.setupTexture(mTexture);
	mTexturePainter.setupRenderPassDesc(mSwapChain->renderPassDescriptor());
	mTexturePainter.setupSampleCount(mSwapChain->sampleCount());
	mTexturePainter.compile();

	mDebugPainter.setupRenderPassDesc(mSwapChain->renderPassDescriptor());
	mDebugPainter.setupSampleCount(mSwapChain->sampleCount());
	mDebugPainter.setupWindow(mSwapChain->window());
	mDebugPainter.compile();
}

void SwapChainRenderPass::execute(QRhiCommandBuffer* cmdBuffer)
{
	QRhiResourceUpdateBatch* batch = RHI->nextResourceUpdateBatch();
	mTexturePainter.resourceUpdate(batch);
	mDebugPainter.resourceUpdate(batch);
	cmdBuffer->resourceUpdate(batch);

	cmdBuffer->beginPass(mSwapChain->currentFrameRenderTarget(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	mTexturePainter.paint(cmdBuffer, mSwapChain->currentFrameRenderTarget());
	mDebugPainter.paint(cmdBuffer, mSwapChain->currentFrameRenderTarget());
	cmdBuffer->endPass();
}