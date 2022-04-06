#ifndef TexturePainter_h__
#define TexturePainter_h__

#include "IPainter.h"

class TexturePainter :public IPainter {
public:
	TexturePainter();

	void setupTexture(QRhiTexture* texture);

	virtual void compile() override;
	virtual void paint(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget) override;
protected:
	QRhiTexture* mTexture;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
};

#endif // TexturePainter_h__
