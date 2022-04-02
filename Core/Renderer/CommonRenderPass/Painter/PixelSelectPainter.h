#ifndef PixelSelectPainter_h__
#define PixelSelectPainter_h__

#include "IPainter.h"

class PixelSelectPainter :public IPainter{
public:
	PixelSelectPainter();
	void setupSelectCode(QByteArray code);
	void setupInputTexture(QRhiTexture* texture);

	virtual void compile() override;
	virtual void paint(QRhiCommandBuffer* cmdBuffer) override;

	QRhiTexture* getOutputTexture() { return mRT.colorAttachment.get(); }
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
	};
	RTResource mRT;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiTexture* mInputTexture;
	QByteArray mSelectCode;
};

#endif // PixelSelectPainter_h__
