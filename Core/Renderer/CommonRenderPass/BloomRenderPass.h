#ifndef BloomRenderPass_h__
#define BloomRenderPass_h__

#include "Painter\BlurPainter.h"
#include "Painter\PixelSelectPainter.h"
#include "Renderer\IRenderPassBase.h"

class QBloomMeragePainter:public IPainter {
public:
	QBloomMeragePainter();

	void setupRenderTarget(QRhiRenderTarget* renderTarget);
	void setupSrcTexutre(QRhiTexture* texture);
	void setupBloomTexutre(QRhiTexture* texture);

	virtual void compile() override;
	virtual void paint(QRhiCommandBuffer* cmdBuffer) override;
private:
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiRenderTarget* mRenderTarget;
	QRhiTexture* mSrcTexture;
	QRhiTexture* mBloomTexture;
};

class BloomRenderPass: public IRenderPassBase {
public:
	BloomRenderPass();
	void setupInputTexture(QRhiTexture* texture);

	virtual void compile() override;
	virtual void execute() override;

	QRhiTexture* getOutputTexture();
	QRhiTexture* getSelectTexture();
	QRhiTexture* getBlurTexture();
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
	};
	RTResource mRT;
	QRhiTexture* mInputTexture;
	BlurPainter mBlurPainter;
	PixelSelectPainter mPixelSelectPainter;
	QBloomMeragePainter mMeragePainter;
};

#endif // BloomRenderPass_h__
