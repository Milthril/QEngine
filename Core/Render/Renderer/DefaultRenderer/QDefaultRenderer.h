#ifndef QDefaultRenderer_h__
#define QDefaultRenderer_h__

#include "Render/Renderer/QSceneRenderer.h"

class QDefaultRenderer :public QSceneRenderer {
public:
	QDefaultRenderer(std::shared_ptr<QRhi> rhi, int sampleCount, std::shared_ptr<QRhiRenderPassDescriptor> renderPassDescriptor) :QSceneRenderer(rhi, sampleCount, renderPassDescriptor) {}

	void render(QRhiCommandBuffer* buffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch) override;

protected:

	std::shared_ptr<QRhiProxy> createShapeProxy(std::shared_ptr<QShapeComponent>) override;

	std::shared_ptr<QRhiProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) override;

	std::shared_ptr<QRhiProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonMeshComponent>) override;

	std::shared_ptr<QRhiProxy> createText2DProxy(std::shared_ptr<QText2DComponent>) override;

	std::shared_ptr<QRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) override;

	std::shared_ptr<QRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) override;
};

#endif // QDefaultRenderer_h__
