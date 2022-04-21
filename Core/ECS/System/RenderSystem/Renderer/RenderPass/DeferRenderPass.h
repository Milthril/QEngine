#ifndef DeferRenderPass_h__
#define DeferRenderPass_h__

#include "IRenderPassBase.h"

class QDeferRenderer;
class QRenderer;

class DeferRenderPass :public IRenderPassBase {
public:
	DeferRenderPass();

	void setupRenderer(QRenderer* renderer);

	virtual void compile() override;

	enum OutputTextureSlot {
		BaseColor = 0,
		Position,
		NormalMetalness,
		TangentRoughness,
		DebugId
	};
	virtual QRhiTexture* getOutputTexture(int slot = 0);

	QVector<QRhiGraphicsPipeline::TargetBlend> getBlendStates() ;
	QRhiRenderTarget* getRenderTarget();
	void execute(QRhiCommandBuffer* cmdBuffer) override;
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> atBaseColor;
		QRhiSPtr<QRhiTexture> atPosition;
		QRhiSPtr<QRhiTexture> atNormal_Metalness;
		QRhiSPtr<QRhiTexture> atTangent_Roughness;
		QRhiSPtr<QRhiTexture> atDebugId;

		QRhiSPtr<QRhiRenderBuffer> atDepthStencil;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;

	};
	RTResource mRT;
	QVector<QRhiGraphicsPipeline::TargetBlend> mBlendStates;

	QRenderer* mRenderer = nullptr;
};

#endif // DeferRenderPass_h__
