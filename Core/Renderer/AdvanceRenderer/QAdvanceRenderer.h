#ifndef QAdvanceRenderer_h__
#define QAdvanceRenderer_h__

#include "Renderer/QSceneRenderer.h"

class QAdvanceRenderer :public QSceneRenderer {
public:
	QAdvanceRenderer(int sampleCount, QRhiSPtr<QRhiRenderPassDescriptor> renderPassDescriptor) :QSceneRenderer(sampleCount, renderPassDescriptor) {}
protected:

	std::shared_ptr<QRhiProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) override;

	std::shared_ptr<QRhiProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) override;

	std::shared_ptr<QRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) override;

	std::shared_ptr<QRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) override;
};

#endif // QAdvanceRenderer_h__