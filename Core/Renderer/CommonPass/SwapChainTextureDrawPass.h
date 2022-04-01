#ifndef SwapChainTextureDrawPass_h__
#define SwapChainTextureDrawPass_h__

#include "Renderer\IRenderPassBase.h"

class SwapChainTextureDrawPass :public IRenderPassBase {
public:
	SwapChainTextureDrawPass();
	void setupTexture(QRhiTexture* texture);
	void setupSwapChain(QRhiSwapChain* swapchain);
	virtual void compile() override;
	virtual void execute() override;
protected:
	QRhiTexture* mTexture;
	QRhiSwapChain* mSwapChain;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
};

#endif // SwapChainTextureDrawPass_h__
