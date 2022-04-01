#ifndef SwapChainRenderPass_h__
#define SwapChainRenderPass_h__

#include "Renderer\IRenderPassBase.h"

class SwapChainRenderPass :public IRenderPassBase {
public:
	SwapChainRenderPass();
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


#endif // SwapChainRenderPass_h__
