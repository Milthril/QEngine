#ifndef ISceneRenderer_h__
#define ISceneRenderer_h__

#include "Scene/QScene.h"
#include "RHI/QRhiDefine.h"
#include "Renderer/QFrameGraph.h"

class IRhiProxy;
class QFrameGraph;

class ISceneRenderer :public QObject {
	Q_OBJECT
public:
	ISceneRenderer();
	void setScene(std::shared_ptr<QScene> scene);
	std::shared_ptr<QScene> getScene() { return mScene; }

	static QShader createShaderFromCode(QShader::Stage stage, const char* code);

	virtual void buildFrameGraph() {}
	virtual void render() {}
	virtual void rebuild() {
		if (mFrameGraph)
			mFrameGraph->compile();
	}
	virtual void requestReadbackCompId(const QPoint& screenPt) {}
Q_SIGNALS:
	void readBackCompId(QSceneComponent::ComponentId);
protected:
	std::shared_ptr<QScene> mScene;
	std::shared_ptr<QFrameGraph> mFrameGraph;
};

#endif // ISceneRenderer_h__
