#ifndef QDefaultProxyStaticMesh_h__
#define QDefaultProxyStaticMesh_h__

#include "QDefaultRenderer.h"

class QParticleComponent;

class QDefaultProxyStaticMesh :public QRhiProxy {

public:
	QDefaultProxyStaticMesh(std::shared_ptr<QStaticMeshComponent> shape);

	std::shared_ptr<QParticleComponent> getParentParticle() const { return mParentParticle; }
	void setParentParticle(std::shared_ptr<QParticleComponent> val) { mParentParticle = val; }
private:
	std::shared_ptr<QStaticMeshComponent> mStaticMesh;
	std::shared_ptr<QParticleComponent> mParentParticle;		//用作粒子实例时生效
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
public:
	void recreateResource() override;
	void recreatePipeline() override;
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
private:
};

#endif // QDefaultProxyStaticMesh_h__
