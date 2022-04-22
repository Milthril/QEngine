#ifndef ForwardRenderPass_h__
#define ForwardRenderPass_h__

#include "IRenderPassBase.h"

class QDeferRenderer;
class QRenderer;

class ForwardRenderPass :public IRenderPassBase {
public:
	ForwardRenderPass();

	void setupDeferColorTexture(QRhiTexture* texture);
	void setupDeferDepthTexture(QRhiTexture* texture);

	virtual void compile() override;

	enum OutputTextureSlot {
		Output
	};

	virtual QRhiTexture* getOutputTexture(int slot = 0);
	QVector<QRhiGraphicsPipeline::TargetBlend> getBlendStates() ;
	QRhiRenderTarget* getRenderTarget();
	void execute(QRhiCommandBuffer* cmdBuffer) override;
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> atBaseColor;
		QRhiSPtr<QRhiTexture> atDebugId;
		QRhiSPtr<QRhiRenderBuffer> atDepthStencil;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	};
	RTResource mRT;
	QRhiTexture* mDeferColorResult;
	QRhiTexture* mDeferDepthResult;
	QVector<QRhiGraphicsPipeline::TargetBlend> mBlendStates;
};

#endif // ForwardRenderPass_h__
