#ifndef BloomPass_h__
#define BloomPass_h__

#include "TextureDrawPass.h"
#include "PixelSelectPass.h"
#include "BlurPass.h"

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

class BloomPass {
public:
	BloomPass();
	void makeBloom(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> inputTexture, QRhiRenderTarget* renderTarget);
	void drawInPass(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget);
private:
	BlurPass mBlurPass;
	PixelSelectPass mPixelSelectPass;
	QBloomMeragePainter mMeragePainter;
};

#endif // BloomPass_h__
