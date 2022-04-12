#ifndef QDefaultSceneRenderPass_h__
#define QDefaultSceneRenderPass_h__

#include "ECS\System\RenderSystem\Renderer\RenderPass\ISceneRenderPass.h"

class QDefaultRenderer;

class QDefaultSceneRenderPass :public ISceneRenderPass {
public:
	QDefaultSceneRenderPass();

	virtual void compile() override;

	QRhiTexture* getOutputTexture() override;
	QRhiTexture* getDebugTexutre() override;

	virtual QVector<QRhiGraphicsPipeline::TargetBlend> getBlendStates() override;
	virtual QRhiRenderTarget* getRenderTarget() override;
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiRenderBuffer> msaaBuffer;
		QRhiSPtr<QRhiRenderBuffer> depthStencil;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;

		QRhiSPtr<QRhiTexture> debugTexture;
		QRhiSPtr<QRhiRenderBuffer> debugMsaaBuffer;
	};
	RTResource mRT;
	QVector<QRhiGraphicsPipeline::TargetBlend> mBlendStates;
};

#endif // QDefaultSceneRenderPass_h__
