#ifndef ISceneRenderPass_h__
#define ISceneRenderPass_h__

#include "IRenderPassBase.h"
#include "Scene\Component\QPrimitiveComponent.h"
#include "IRhiProxy.h"
#include "Scene\Component\StaticMesh\QStaticMeshComponent.h"
#include "Scene\Component\SkeletonMesh\QSkeletonMeshComponent.h"
#include "Scene\Component\Particle\QParticleComponent.h"
#include "Scene\Component\SkyBox\QSkyBoxComponent.h"

class ISceneRenderPass :public IRenderPassBase {
public:
	ISceneRenderPass(std::shared_ptr<QScene> scene);

	std::shared_ptr<IRhiProxy> createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component);
	virtual void execute() override final;
	virtual std::shared_ptr<IRhiProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) = 0;
	virtual std::shared_ptr<IRhiProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) = 0;
	virtual std::shared_ptr<IRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<IRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) = 0;

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
	QHash<QSceneComponent::ComponentId, std::shared_ptr<IRhiProxy>> mPrimitiveProxyMap;
	std::shared_ptr<QScene> mScene;
};

#endif // ISceneRenderPass_h__
