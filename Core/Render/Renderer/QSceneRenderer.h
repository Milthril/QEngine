#ifndef QSceneRenderer_h__
#define QSceneRenderer_h__

#include "Render/Scene/QScene.h"

class QRhiProxy {
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};

class QPrimitiveComponent;
class QShapeComponent;
class QStaticMeshComponent;
class QSkeletonMeshComponent;
class QText2DComponent;
class QParticleComponent;
class QSkyBoxComponent;

class QSceneRenderer {
public:
	QSceneRenderer(std::shared_ptr<QRhi> rhi) :mRhi(rhi) {}
	void setScene(std::shared_ptr<QScene> scene) { mScene = scene; }
	std::shared_ptr<QScene> getScene() { return mScene; }
protected:
	std::shared_ptr<QRhiGraphicsPipeline> createPrimitivePipline(std::shared_ptr<QPrimitiveComponent> component);
	virtual std::shared_ptr<QRhiGraphicsPipeline> createShapePipline(std::shared_ptr<QShapeComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> createStaticMeshPipline(std::shared_ptr<QStaticMeshComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> createSkeletonMeshPipline(std::shared_ptr<QSkeletonMeshComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> createText2DPipline(std::shared_ptr<QText2DComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> createParticlePipline(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> createSkyBoxPipline(std::shared_ptr<QSkyBoxComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createRhiProxy(std::shared_ptr<QSceneComponent> component) = 0;
private:
	std::shared_ptr<QRhi> mRhi;
	std::shared_ptr<QScene> mScene;
	QHash<QSceneComponent::Type, std::shared_ptr<QRhiGraphicsPipeline>> mGraphicsPipelineMap;
};

#endif // QSceneRenderer_h__
