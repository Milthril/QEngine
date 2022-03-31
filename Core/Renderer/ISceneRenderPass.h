#ifndef ISceneRenderPass_h__
#define ISceneRenderPass_h__

#include "IRenderPassBase.h"
#include "Scene\Component\QPrimitiveComponent.h"
#include "ISceneComponentRenderProxy.h"
#include "Scene\Component\StaticMesh\QStaticMeshComponent.h"
#include "Scene\Component\SkeletonMesh\QSkeletonMeshComponent.h"
#include "Scene\Component\Particle\QParticleComponent.h"
#include "Scene\Component\SkyBox\QSkyBoxComponent.h"

class ISceneRenderPass :public IRenderPassBase {
public:
	ISceneRenderPass() {}

	std::shared_ptr<ISceneComponentRenderProxy> createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component) {
		if (!component)
			return nullptr;
		std::shared_ptr<ISceneComponentRenderProxy> proxy;
		switch (component->type())
		{
		case QSceneComponent::None:
			return nullptr;
			break;
		case QSceneComponent::StaticMesh:
			proxy = createStaticMeshProxy(std::dynamic_pointer_cast<QStaticMeshComponent>(component));
			break;
		case QSceneComponent::SkeletonMesh:
			proxy = createSkeletonMeshProxy(std::dynamic_pointer_cast<QSkeletonModelComponent>(component));
			break;
		case QSceneComponent::Particle:
			proxy = createParticleProxy(std::dynamic_pointer_cast<QParticleComponent>(component));
			break;
		default:
			return nullptr;
			break;
		}
		proxy->mComponent = component;
		component->bNeedRecreateResource.active();
		component->bNeedRecreatePipeline.active();
		return std::dynamic_pointer_cast<ISceneComponentRenderProxy>(proxy);
	}

	virtual std::shared_ptr<ISceneComponentRenderProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) = 0;

	virtual int getSampleCount() = 0;
	virtual QRhiRenderPassDescriptor* getRenderPassDescriptor() = 0;
	virtual QVector<QRhiGraphicsPipeline::TargetBlend>  getBlendStates() = 0;

	bool getEnableOutputDebugId() const { return mEnableOutputDebugId; }
	void setEnableOutputDebugId(bool val) { mEnableOutputDebugId = val; }
protected:
	bool mEnableOutputDebugId = false;
};

#endif // ISceneRenderPass_h__
