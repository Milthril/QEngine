#ifndef QDefaultRenderer_h__
#define QDefaultRenderer_h__

#include "Renderer/ISceneRenderer.h"
#include "Renderer/CommonPass/BloomPass.h"

class QDefaultRenderer :public ISceneRenderer {
public:
	QDefaultRenderer();
	virtual void render() override;
	virtual void requestReadbackCompId(const QPoint& screenPt) override;
protected:
	std::shared_ptr<ISceneComponentRenderProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) override;
	std::shared_ptr<ISceneComponentRenderProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) override;
	std::shared_ptr<ISceneComponentRenderProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) override;
	std::shared_ptr<ISceneComponentRenderProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) override;
private:
	QRhiReadbackResult mReadReult;
	QRhiReadbackDescription mReadDesc;
	QPoint mReadPoint;
};

#endif // QDefaultRenderer_h__
