#ifndef ISceneRenderer_h__
#define ISceneRenderer_h__

#include "Scene/QScene.h"
#include "CommonPass/BloomPass.h"
#include "RHI/QRhiDefine.h"

class QPrimitiveComponent;
class QStaticMeshComponent;
class QStaticMeshComponent;
class QSkeletonModelComponent;
class QParticleComponent;
class QSkyBoxComponent;
class ISceneRenderer;
class DebugDrawPass;
class ISceneComponentRenderProxy;

class ISceneRenderer :public QObject {
	Q_OBJECT
public:
	ISceneRenderer();
	void setScene(std::shared_ptr<QScene> scene);
	std::shared_ptr<QScene> getScene() { return mScene; }

	std::shared_ptr<ISceneComponentRenderProxy> createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component);
	static QShader createShaderFromCode(QShader::Stage stage, const char* code);
	virtual void render() {}
	virtual void requestReadbackCompId(const QPoint& screenPt) {}
Q_SIGNALS:
	void readBackCompId(QSceneComponent::ComponentId);
private:
	virtual std::shared_ptr<ISceneComponentRenderProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<ISceneComponentRenderProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) = 0;
protected:
	std::shared_ptr<QScene> mScene;
};

#endif // ISceneRenderer_h__
