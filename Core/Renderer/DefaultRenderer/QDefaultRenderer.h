#ifndef QDefaultRenderer_h__
#define QDefaultRenderer_h__

#include "Renderer/ISceneRenderer.h"
#include "QDefaultSceneRenderPass.h"
#include "Renderer/CommonPass/SwapChainTextureDrawPass.h"

class QDefaultRenderer :public ISceneRenderer {
public:
	QDefaultRenderer();
	virtual void setup() override;
	virtual void render() override;
	virtual void requestReadbackCompId(const QPoint& screenPt) override;
protected:
	QRhiReadbackResult mReadReult;
	QRhiReadbackDescription mReadDesc;
	QPoint mReadPoint;
	QDefaultSceneRenderPass mScenePass;
	SwapChainTextureDrawPass mSwapChainDrawPass;
};

#endif // QDefaultRenderer_h__
