#ifndef TexturePainter_h__
#define TexturePainter_h__

#include "Renderer\IRenderPassBase.h"

class TexturePainter :public IRenderPassBase {
public:
	TexturePainter();

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

#endif // TexturePainter_h__
