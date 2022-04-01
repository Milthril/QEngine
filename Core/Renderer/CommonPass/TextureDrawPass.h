#ifndef TextureDrawPass_h__
#define TextureDrawPass_h__

#include "Renderer\IRenderPassBase.h"

class TextureDrawPass :public IRenderPassBase {
public:
	TextureDrawPass();

	void setupRenderTarget(QRhiRenderTarget* renderTarget);
	void setupCmdBuffer(QRhiCommandBuffer* cmdBuffer);
	void setupTexture(QRhiTexture* texture);

	virtual void compile() override;
	virtual void execute() override;
protected:
	QRhiRenderTarget* mRenderTarget;
	QRhiCommandBuffer* mCmdBuffer;
	QRhiTexture* mTexture;

	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
};

#endif // TextureDrawPass_h__
