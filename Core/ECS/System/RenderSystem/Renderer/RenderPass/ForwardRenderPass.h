#ifndef ForwardRenderPass_h__
#define ForwardRenderPass_h__

#include "IRenderPassBase.h"

class QDeferRenderer;
class QRenderer;

class ForwardRenderPass :public IRenderPassBase {
public:
	ForwardRenderPass();

	virtual void compile() override;

	enum InputTextureSlot {
		Color = 0,
		Depth,
		DeferDebugId
	};

	enum OutputTextureSlot {
		Output = 0,
		DebugId
	};

	QVector<QRhiGraphicsPipeline::TargetBlend> getBlendStates() ;
	QRhiRenderTarget* getRenderTarget();
	void execute(QRhiCommandBuffer* cmdBuffer) override;
private:
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
