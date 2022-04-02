#ifndef SwapChainRenderPass_h__
#define SwapChainRenderPass_h__

#include "Renderer\IRenderPassBase.h"
#include "Painter\DebugPainter.h"
#include "Painter\TexturePainter.h"

class SwapChainRenderPass :public IRenderPassBase {
public:
	SwapChainRenderPass();
	void setupDebugTexture(QRhiTexture* texture);
	void setupTexture(QRhiTexture* texture);
	void setupSwapChain(QRhiSwapChain* swapchain);
	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;
protected:
	QRhiTexture* mTexture;
	QRhiSwapChain* mSwapChain;

	TexturePainter mTexturePainter;
	DebugPainter mDebugPainter;
};

#endif // SwapChainRenderPass_h__
