#ifndef IRenderer_h__
#define IRenderer_h__

#include "QFrameGraph.h"
#include "ECS/QWorld.h"

class QFrameGraph;
class ISceneRenderPass;

class IRenderer :public QObject {
	friend class QRenderSystem;
	Q_OBJECT
public:
	IRenderer(std::shared_ptr<ISceneRenderPass> scenePass);

	virtual void buildFrameGraph() {}
	virtual void render(QRhiCommandBuffer* cmdBuffer) {}

	virtual void rebuild() {
		if (mFrameGraph)
			mFrameGraph->compile();
	}
	virtual void requestReadbackCompId(const QPoint& screenPt) {}
	std::shared_ptr<ISceneRenderPass> getScenePass() const { return mScenePass; }

Q_SIGNALS:
	void readBackEntityId(QEntity::ID);
protected:
	std::shared_ptr<QFrameGraph> mFrameGraph;
	std::shared_ptr<ISceneRenderPass> mScenePass;

};

#endif // IRenderer_h__
