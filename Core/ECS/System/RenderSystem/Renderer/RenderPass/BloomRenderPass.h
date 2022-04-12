#ifndef BloomRenderPass_h__
#define BloomRenderPass_h__

#include "BlurRenderPass.h"
#include "IRenderPassBase.h"


class BloomMerageRenderPass:public IRenderPassBase {
public:
	BloomMerageRenderPass();
	void setupSrcTexutre(QRhiTexture* texture);
	void setupBloomTexutre(QRhiTexture* texture);

	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;
	virtual QRhiTexture* getOutputTexture() override { return mRT.colorAttachment.get(); }
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
