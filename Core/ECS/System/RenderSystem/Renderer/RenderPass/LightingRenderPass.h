#ifndef LightingRenderPass_h__
#define LightingRenderPass_h__

#include "BlurRenderPass.h"
#include "IRenderPassBase.h"


class LightingRenderPass:public IRenderPassBase {
public:
	LightingRenderPass();

	void setupBaseColorTexutre(QRhiTexture* texture);
	void setupPositionTexutre(QRhiTexture* texture);
	void setupNormalMetalnessTexutre(QRhiTexture* texture);
	void setupTangentRoughnessTexutre(QRhiTexture* texture);
	
	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	enum OutputTextureSlot {
		LightingResult
	};
	virtual QRhiTexture* getOutputTexture(int slot = 0) {
		switch ((OutputTextureSlot)slot) {
		case LightingResult: return mRT.colorAttachment.get();
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

	QRhiTexture* mBaseColorTexture;
	QRhiTexture* mPositionTexture;
	QRhiTexture* mNormal_MetalnessTexture;
	QRhiTexture* mTangent_RoughnessTexture;
};

#endif // LightingRenderPass_h__
