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
	ISceneRenderPass(ISceneRenderer* renderer);

	std::shared_ptr<ISceneComponentRenderProxy> createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component);
	virtual void execute() override final;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) = 0;

	virtual QVector<QRhiGraphicsPipeline::TargetBlend>  getBlendStates() = 0;
	virtual QRhiRenderTarget* getRenderTarget() = 0;

	int getSampleCount() {
		return getRenderTarget()->sampleCount();
	}
	QRhiRenderPassDescriptor* getRenderPassDescriptor() {
		return getRenderTarget()->renderPassDescriptor();
	}
	bool getEnableOutputDebugId() const { return mEnableOutputDebugId; }
	void setEnableOutputDebugId(bool val) { mEnableOutputDebugId = val; }

protected:
	bool mEnableOutputDebugId = false;
	QHash<QSceneComponent::ComponentId, std::shared_ptr<ISceneComponentRenderProxy>> mPrimitiveProxyMap;
	ISceneRenderer* mRenderer = nullptr;
};

#endif // ISceneRenderPass_h__
