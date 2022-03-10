#ifndef QDefaultRenderer_h__
#define QDefaultRenderer_h__

#include "Render/Renderer/QSceneRenderer.h"
#include "../Common/QTexturePainter.h"
#include "../Common/QBloomPainter.h"

class QDefaultRenderer :public QSceneRenderer {
public:
	QDefaultRenderer(std::shared_ptr<QRhi> rhi, int sampleCount, QRhiSPtr<QRhiRenderPassDescriptor> renderPassDescriptor);
	virtual void render(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget) override;
protected:
	std::shared_ptr<QRhiProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) override;
	std::shared_ptr<QRhiProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonMeshComponent>) override;
	std::shared_ptr<QRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) override;
	std::shared_ptr<QRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) override;
	void createOrResizeRenderTarget(QSize size);
	QRhiSPtr<QRhiRenderPassDescriptor> getRenderPassDescriptor() const override;
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiRenderBuffer> msaaBuffer;
		QRhiSPtr<QRhiRenderBuffer> depthStencil;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	};
	RTResource mRT;
};

#endif // QDefaultRenderer_h__
