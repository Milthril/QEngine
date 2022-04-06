#ifndef BloomRenderPass_h__
#define BloomRenderPass_h__

#include "Painter\BlurPainter.h"
#include "Painter\PixelSelectPainter.h"
#include "Renderer\IRenderPassBase.h"

class BloomMerageRenderPass:public IRenderPassBase {
public:
	BloomMerageRenderPass();
	void setupSrcTexutre(QRhiTexture* texture);
	void setupBloomTexutre(QRhiTexture* texture);

	QRhiTexture* getOutputTexture() { return mRT.colorAttachment.get(); }
	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	};
	RTResource mRT;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiTexture* mSrcTexture;
	QRhiTexture* mBloomTexture;
};

#endif // BloomRenderPass_h__
