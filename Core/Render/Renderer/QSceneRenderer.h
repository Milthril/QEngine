#ifndef QSceneRenderer_h__
#define QSceneRenderer_h__

#include "Render/Scene/QScene.h"

class QRhiProxy {
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};

class QPrimitiveComponent;
class QStaticMeshComponent;
class QSkeletonMeshComponent;
class QText2DComponent;
class QParticleComponent;
class QSkyBoxComponent;

class QSceneRenderer {
public:
	QSceneRenderer(std::shared_ptr<QRhi> rhi) :mRhi(rhi) {}
protected:
	virtual void SetupScene(QScene scene) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> PrimitivePipline(std::shared_ptr<QPrimitiveComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> StaticMeshPipline(std::shared_ptr<QStaticMeshComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> SkeletonMeshPipline(std::shared_ptr<QSkeletonMeshComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> Text2DPipline(std::shared_ptr<QText2DComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> ParticlePipline(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<QRhiGraphicsPipeline> SkyBoxPipline(std::shared_ptr<QPrimitiveComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> CreateProxy(std::shared_ptr<QSkyBoxComponent> component) = 0;
private:
	std::shared_ptr<QRhi> mRhi;
	QHash<QSceneComponent::Type, std::shared_ptr<QRhiGraphicsPipeline>> mGraphicsPipelineMap;
};

#endif // QSceneRenderer_h__
