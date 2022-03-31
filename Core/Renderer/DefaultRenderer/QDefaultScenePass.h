#ifndef QDefaultScenePass_h__
#define QDefaultScenePass_h__

#include "Scene\QSceneComponent.h"
#include "Renderer\ISceneRenderPass.h"

class ISceneComponentRenderProxy;

class QDefaultScenePass :public QSceneRenderPass {
public:
	QDefaultScenePass(QDefaultRenderer* renderer);
	virtual void compile() override;
	virtual void execute() override;

	void setupSceneFrameSize(QSize size);
	void setupSampleCount(int count);

	QRhiSPtr<QRhiTexture> getOutputTexture();
	virtual int getSampleCount() override;
	virtual QRhiRenderPassDescriptor* getRenderPassDescriptor() override;
	virtual QVector<QRhiGraphicsPipeline::TargetBlend> getBlendStates() override;
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
	QDefaultRenderer* mRenderer = nullptr;

	QRhiReadbackResult mReadReult;
	QRhiReadbackDescription mReadDesc;
	QPoint mReadPoint;
	QHash<QSceneComponent::ComponentId, std::shared_ptr<ISceneComponentRenderProxy>> mPrimitiveProxyMap;
};

#endif // QDefaultScenePass_h__
