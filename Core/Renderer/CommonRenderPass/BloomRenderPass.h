#ifndef BloomRenderPass_h__
#define BloomRenderPass_h__

#include "Painter\BlurPainter.h"
#include "Painter\PixelSelectPainter.h"

class QBloomMeragePainter {
public:
	QBloomMeragePainter();
	void updateTexture(QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom, QRhiRenderTarget* renderTarget);
	void drawInPass(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget);
protected:
	void initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom);
private:
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mSrcTexture;
	QRhiSPtr<QRhiTexture> mBloomTexture;
};

class BloomRenderPass {
public:
	BloomRenderPass();
	void makeBloom(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> inputTexture, QRhiRenderTarget* renderTarget);
	void drawInPass(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget);
private:
	BlurPainter mBlurPainter;
	PixelSelectPainter mPixelSelectPainter;
	QBloomMeragePainter mMeragePainter;
};

#endif // BloomRenderPass_h__
