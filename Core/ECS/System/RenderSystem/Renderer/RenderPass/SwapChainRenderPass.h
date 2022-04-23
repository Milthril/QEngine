#ifndef SwapChainRenderPass_h__
#define SwapChainRenderPass_h__

#include "Painter\DebugPainter.h"
#include "Painter\TexturePainter.h"
#include "IRenderPassBase.h"

class SwapChainRenderPass :public IRenderPassBase {
public:
	SwapChainRenderPass();
	void setupDebugTexture(QRhiTexture* texture);
	void setupTexture(QRhiTexture* texture);
	void setupSwapChain(QRhiSwapChain* swapchain);

	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	virtual QRhiTexture* getOutputTexture(int slot = 0) {
		return nullptr;
	}
protected:
	QRhiTexture* mTexture;
	QRhiSwapChain* mSwapChain;

	TexturePainter mTexturePainter;
	DebugPainter mDebugPainter;
};

#endif // SwapChainRenderPass_h__
