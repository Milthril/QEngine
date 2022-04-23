#include "QRenderWindow.h"
#include "QRenderSystem.h"
QRenderWindow::QRenderWindow(QRhi::Implementation backend)
	: QRhiWindow(backend)
{
}

void QRenderWindow::customInitEvent() {
	TheRenderSystem->setupRHI(this->mRhi);
	if (TheRenderSystem->renderer()) {
		TheRenderSystem->renderer()->buildFrameGraph();
	}
	mDurationMs = 0;
	mLastTimeMs = 0;
	mTimer.restart();
}

void QRenderWindow::customRenderEvent(QRhiSwapChain* swapchain) {

	if (TheRenderSystem->renderer()) {
		if (mRhi->beginFrame(swapchain) == QRhi::FrameOpSuccess) {
			swapchain->currentFrameRenderTarget()->setRenderPassDescriptor(swapchain->renderPassDescriptor());
			TheRenderSystem->renderer()->render(swapchain->currentFrameCommandBuffer());
			mRhi->endFrame(swapchain);
		}
	}
	qint64 current = mTimer.elapsed();
	mDurationMs = current - mLastTimeMs;
	mLastTimeMs = current;
}

void QRenderWindow::customResizeEvent() {
	if (TheRenderSystem->renderer()) {
		TheRenderSystem->renderer()->resize(mSwapChain->currentPixelSize());
	}
}
