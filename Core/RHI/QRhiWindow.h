#ifndef QRhiWindow_h__
#define QRhiWindow_h__

#include <QWindow>
#include <QOffscreenSurface>
#include "Renderer/DefaultRenderer/QDefaultRenderer.h"

class QRhiWindow :public QWindow {
	friend class QEngine;
public:
	QRhiWindow(QRhi::Implementation backend);
	void waitExposed();
	int getFPS() const { return mFPS; }
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
	QElapsedTimer mTimer;
	int mFrameCount = 0;
	int mFPS = 0;
protected:
	bool mRunning = false;
	bool mNotExposed = false;
	bool mNewlyExposed = false;
	bool mHasSwapChain = false;
};

#endif // QRhiWindow_h__
