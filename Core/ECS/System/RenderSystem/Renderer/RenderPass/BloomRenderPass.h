#ifndef BloomRenderPass_h__
#define BloomRenderPass_h__

#include "BlurRenderPass.h"
#include "IRenderPassBase.h"


class BloomMerageRenderPass:public IRenderPassBase {
public:
	BloomMerageRenderPass();

	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	enum InputTextureSlot {
		Src = 0,
		Bloom
	};

	enum OutputTextureSlot {
		BloomMerageResult = 0,
	};

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
};

#endif // BloomRenderPass_h__
