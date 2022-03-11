#ifndef QPixelSelector_h__
#define QPixelSelector_h__

#include "RHI\QRhiDefine.h"

class QPixelSelector {
public:
	QPixelSelector(std::shared_ptr<QRhi> rhi, QByteArray code);
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> texture, QRhiRenderTarget* renderTarget);
protected:
	void initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> texture);
	void updateTexture(QRhiSPtr<QRhiTexture> texture);
private:
	std::shared_ptr<QRhi> mRhi;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mTexture;
	QByteArray mSelectCode;
};

#endif // QPixelSelector_h__
