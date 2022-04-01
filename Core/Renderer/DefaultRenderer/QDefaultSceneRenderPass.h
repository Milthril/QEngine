#ifndef QDefaultSceneRenderPass_h__
#define QDefaultSceneRenderPass_h__

#include "Scene\QSceneComponent.h"
#include "Renderer\ISceneRenderPass.h"
#include "Renderer\ISceneComponentRenderProxy.h"

class QDefaultRenderer;

class QDefaultSceneRenderPass :public ISceneRenderPass {
public:
	QDefaultSceneRenderPass(QDefaultRenderer* renderer);
	std::shared_ptr<ISceneComponentRenderProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) override;
	std::shared_ptr<ISceneComponentRenderProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) override;
	std::shared_ptr<ISceneComponentRenderProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) override;
	std::shared_ptr<ISceneComponentRenderProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) override;

	virtual void compile() override;

	void setupSceneFrameSize(QSize size);
	void setupSampleCount(int count);

	QRhiTexture* getOutputTexture();

	virtual QVector<QRhiGraphicsPipeline::TargetBlend> getBlendStates() override;
	virtual QRhiRenderTarget* getRenderTarget() override;
private:
	QSize mSceneFrameSize;
	int mSampleCount = 1;
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
	QRhiReadbackResult mReadReult;
	QRhiReadbackDescription mReadDesc;
	QPoint mReadPoint;

	QVector<QRhiGraphicsPipeline::TargetBlend> mBlendStates;
};

#endif // QDefaultSceneRenderPass_h__
