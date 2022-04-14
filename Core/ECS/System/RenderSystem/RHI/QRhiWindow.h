#ifndef QRhiWindow_h__
#define QRhiWindow_h__

#include <QWindow>
#include <QOffscreenSurface>
#include "QRhiDefine.h"

class QRhiWindow :public QWindow {
	friend class QRenderSystem;
public:
	QRhiWindow(QRhi::Implementation backend);
	void waitExposed();
	int getFPS() const { return mFPS; }
	bool hasClosed() const { return mHasClosed; }
	QRhiSwapChain* getSwapChain() { return mSwapChain.get(); };
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
	bool mHasClosed = false;

};

#endif // QRhiWindow_h__