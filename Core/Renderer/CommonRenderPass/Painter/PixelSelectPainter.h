#ifndef PixelSelectPainter_h__
#define PixelSelectPainter_h__

#include "RHI\QRhiDefine.h"

class PixelSelectPainter {
public:
	PixelSelectPainter(QByteArray code);
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> texture, QRhiRenderTarget* renderTarget);
protected:
	void initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> texture);
	void updateTexture(QRhiSPtr<QRhiTexture> texture);
private:
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mTexture;
	QByteArray mSelectCode;
};

#endif // PixelSelectPainter_h__
