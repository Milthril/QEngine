#ifndef QDefaultRenderer_h__
#define QDefaultRenderer_h__

#include "Render/Renderer/QSceneRenderer.h"
#include "../Common/QFullSceneTexturePainter.h"

class QDefaultRenderer :public QSceneRenderer {
public:
	QDefaultRenderer(std::shared_ptr<QRhi> rhi, int sampleCount, std::shared_ptr<QRhiRenderPassDescriptor> renderPassDescriptor);

	virtual void render(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch) override;

protected:
	std::shared_ptr<QRhiProxy> createShapeProxy(std::shared_ptr<QShapeComponent>) override;
	std::shared_ptr<QRhiProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) override;
	std::shared_ptr<QRhiProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonMeshComponent>) override;
	std::shared_ptr<QRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) override;
	std::shared_ptr<QRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) override;
	void createOrResizeRenderTarget(QSize size);
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiRenderBuffer> depthStencil;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	};
	RTResource mRT;
	std::shared_ptr<QFullSceneTexturePainter> mFSPainter;
};

#endif // QDefaultRenderer_h__
