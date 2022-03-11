#ifndef QTexturePainter_h__
#define QTexturePainter_h__

#include "RHI\QRhiDefine.h"

class QTexturePainter {
public:
	QTexturePainter();
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> texture, QRhiRenderTarget* renderTarget);
protected:
	void initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> texture);
	void updateTexture(QRhiSPtr<QRhiTexture> texture);
private:
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mTexture;
};

#endif // QTexturePainter_h__
