#ifndef QDefaultScenePass_h__
#define QDefaultScenePass_h__

#include "Renderer\FrameGraph\QRenderPass.h"
#include "Scene\QSceneComponent.h"

class QDefaultRenderer;
class QRhiProxy;

class QDefaultScenePass :public QRenderPass {
public:
	QDefaultScenePass(QDefaultRenderer* renderer);
	virtual void compile() override;
	virtual void execute() override;

	void setupEnableOutputID(bool var);
	void setupSceneFrameSize(QSize size);
	void setupSampleCount(int count);
	QRhiSPtr<QRhiTexture> getOutputTexture();

private:
	bool mEnableOutputID = false;
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
	QHash<QSceneComponent::ComponentId, std::shared_ptr<QRhiProxy>> mPrimitiveProxyMap;
};

#endif // QDefaultScenePass_h__
