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

	enum OutputTextureSlot {
		BloomMerageResult = 0,
	};
	virtual QRhiTexture* getOutputTexture(int slot = 0) {
		switch ((OutputTextureSlot)slot) {
		case BloomMerageResult:
			return mRT.colorAttachment.get();
		}
		return nullptr;
	}

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
