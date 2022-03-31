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

	static QShader createShaderFromCode(QShader::Stage stage, const char* code);
	virtual void render() {}
	virtual void requestReadbackCompId(const QPoint& screenPt) {}
Q_SIGNALS:
	void readBackCompId(QSceneComponent::ComponentId);
private:
protected:
	std::shared_ptr<QScene> mScene;
};

#endif // ISceneRenderer_h__
