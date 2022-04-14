#ifndef QRenderWindow_h__
#define QRenderWindow_h__

#include "RHI\QRhiWindow.h"

class QRenderWindow:public QRhiWindow {
	friend class QRenderSystem;
public:
	QRenderWindow(QRhi::Implementation backnd);
	virtual void customInitEvent() override;
	virtual void customRenderEvent(QRhiSwapChain* swapchain) override;
	virtual void customResizeEvent() override;
};

#endif // QRenderWindow_h__
