#ifndef SwapChainRenderPass_h__
#define SwapChainRenderPass_h__

#include "Painter\DebugPainter.h"
#include "Painter\TexturePainter.h"
#include "IRenderPassBase.h"

class SwapChainRenderPass :public IRenderPassBase {
public:
	SwapChainRenderPass();

	void setupSwapChain(QRhiSwapChain* swapchain);

	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	enum InputTextureSlot {
		Color = 0,
		DebugId,
	};

protected:
	QRhiSwapChain* mSwapChain;

	TexturePainter mTexturePainter;
	DebugPainter mDebugPainter;
};

#endif // SwapChainRenderPass_h__
