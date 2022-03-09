#ifndef QRhiWindow_h__
#define QRhiWindow_h__

#include <QWindow>
#include <QOffscreenSurface>
#include "private\qrhi_p.h"
#include "Render\Renderer\QSceneRenderer.h"

class QRhiWindow :public QWindow {
public:
	QRhiWindow(QRhi::Implementation backend);
	static void setDefaultSurfaceFormat(QSurfaceFormat format);
	std::shared_ptr<QScene> scene() const { return mScene; }
	void waitExposed();
private:
	void initInternal();
	void renderInternal();
	void resizeSwapChain();
protected:
	void exposeEvent(QExposeEvent*) override;
	bool event(QEvent*) override;
private:
	QRhi::Implementation mBackend;
	std::shared_ptr<QRhi> mRhi;
	QRhiSPtr<QRhiSwapChain> mSwapChain;
	QRhiSPtr<QRhiRenderBuffer> mDepthStencilFrameBuffer;
	QRhiSPtr<QRhiRenderPassDescriptor> mRenderPassDesciptor;
	std::shared_ptr<QOffscreenSurface> mFallbackSurface;
	std::shared_ptr<QSceneRenderer>  mRootRenderer;
	std::shared_ptr<QScene> mScene;
	QElapsedTimer mTimer;
	int mFrameCount = 0;
protected:
	bool mRunning = false;
	bool mNotExposed = false;
	bool mNewlyExposed = false;
	bool mHasSwapChain = false;
};

#endif // QRhiWindow_h__
