#ifndef QDefaultRenderer_h__
#define QDefaultRenderer_h__

#include "Renderer/QSceneRenderer.h"
#include "Renderer/CommonPass/BloomPass.h"

class QDefaultRenderer :public QSceneRenderer {
public:
	QDefaultRenderer();
	virtual void render(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget) override;
	virtual QVector<QRhiGraphicsPipeline::TargetBlend> getDefaultBlends() override;
	virtual void requestReadbackCompId(const QPoint& screenPt) override;
protected:
	void createOrResizeRenderTarget(QSize size);
	std::shared_ptr<QRhiProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) override;
	std::shared_ptr<QRhiProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) override;
	std::shared_ptr<QRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) override;
	std::shared_ptr<QRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) override;
	QRhiSPtr<QRhiRenderPassDescriptor> getRenderPassDescriptor() const override;
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

	std::shared_ptr<BloomPass> mBloomPainter;
	QRhiReadbackResult mReadReult;
	QRhiReadbackDescription mReadDesc;
	QPoint mReadPoint;
};

#endif // QDefaultRenderer_h__
