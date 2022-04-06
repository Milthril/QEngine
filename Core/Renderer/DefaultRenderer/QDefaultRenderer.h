#ifndef QDefaultRenderer_h__
#define QDefaultRenderer_h__

#include "Renderer/ISceneRenderer.h"


class QDefaultRenderer :public ISceneRenderer {
public:
	QDefaultRenderer();
	virtual void buildFrameGraph() override;
	virtual void render(QRhiCommandBuffer* cmdBuffer) override;
	virtual void requestReadbackCompId(const QPoint& screenPt) override;
};

#endif // QDefaultRenderer_h__
