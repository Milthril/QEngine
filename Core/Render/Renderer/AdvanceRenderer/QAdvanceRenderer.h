#ifndef QAdvanceRenderer_h__
#define QAdvanceRenderer_h__

#include "Render/Renderer/QSceneRenderer.h"

class QAdvanceRenderer :public QSceneRenderer {
public:
	QAdvanceRenderer(std::shared_ptr<QRhi> rhi) :QSceneRenderer(rhi) {}
protected:
	std::shared_ptr<QRhiGraphicsPipeline> createShapePipline(std::shared_ptr<QShapeComponent>) override;
	std::shared_ptr<QRhiGraphicsPipeline> createStaticMeshPipline(std::shared_ptr<QStaticMeshComponent>) override;
	std::shared_ptr<QRhiGraphicsPipeline> createSkeletonMeshPipline(std::shared_ptr<QSkeletonMeshComponent>) override;
	std::shared_ptr<QRhiGraphicsPipeline> createText2DPipline(std::shared_ptr<QText2DComponent>) override;
	std::shared_ptr<QRhiGraphicsPipeline> createParticlePipline(std::shared_ptr<QParticleComponent>) override;
	std::shared_ptr<QRhiGraphicsPipeline> createSkyBoxPipline(std::shared_ptr<QSkyBoxComponent>) override;
	std::shared_ptr<QRhiProxy> createRhiProxy(std::shared_ptr<QSceneComponent> component) override;
};

#endif // QAdvanceRenderer_h__