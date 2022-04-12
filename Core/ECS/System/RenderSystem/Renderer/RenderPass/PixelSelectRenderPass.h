#ifndef PixelSelectRenderPass_h__
#define PixelSelectRenderPass_h__

#include "IRenderPassBase.h"

class PixelSelectRenderPass :public IRenderPassBase{
public:
	PixelSelectRenderPass();
	void setupSelectCode(QByteArray code);
	void setupInputTexture(QRhiTexture* texture);
	void setDownSamplerCount(int count);
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
	QRhiTexture* mInputTexture;
	QByteArray mSelectCode;
	uint32_t mDownSamplerCount = 1;
};

#endif // PixelSelectRenderPass_h__
