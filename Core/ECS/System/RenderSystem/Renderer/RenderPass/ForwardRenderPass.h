#ifndef ForwardRenderPass_h__
#define ForwardRenderPass_h__

#include "IRenderPassBase.h"

class QDeferRenderer;
class QRenderer;

class ForwardRenderPass :public IRenderPassBase {
public:
	ForwardRenderPass();

	void setupInputColorTexture(QRhiTexture* texture);
	void setupInputDepthTexture(QRhiTexture* texture);
	void setupInputDebugIdTexture(QRhiTexture* texture);

	virtual void compile() override;

	enum OutputTextureSlot {
		Output = 0,
		DebugId
	};

	virtual QRhiTexture* getOutputTexture(int slot = 0);
	QVector<QRhiGraphicsPipeline::TargetBlend> getBlendStates() ;
	QRhiRenderTarget* getRenderTarget();
	void execute(QRhiCommandBuffer* cmdBuffer) override;
private:
	QRhiTexture* mInputColorTexture = nullptr;
	QRhiTexture* mInputDepthTexture = nullptr;
	QRhiTexture* mInputDebugIdTexture = nullptr;

	QRhiSPtr<QRhiGraphicsPipeline> mCopyPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;

	struct RTResource {
		QRhiSPtr<QRhiTexture> atBaseColor;
		QRhiSPtr<QRhiRenderBuffer> atDepth;
		QRhiSPtr<QRhiTexture> atDebugId;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	};
	RTResource mRT;
	QVector<QRhiGraphicsPipeline::TargetBlend> mBlendStates;
};

#endif // ForwardRenderPass_h__
