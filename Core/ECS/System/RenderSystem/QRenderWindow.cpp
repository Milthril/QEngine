#include "QRenderWindow.h"
#include "QRenderSystem.h"
#include "Renderer/IRenderer.h"

QRenderWindow::QRenderWindow(QRhi::Implementation backend)
	: QRhiWindow(backend)
{
}

void QRenderWindow::customInitEvent() {
	QRenderSystem::instance()->setupRHI(this->mRhi);
	if (QRenderSystem::instance()->renderer()) {
		QRenderSystem::instance()->renderer()->buildFrameGraph();
	}
	mDurationMs = 0;
	mLastTimeMs = 0;
	mTimer.restart();
}

void QRenderWindow::customRenderEvent(QRhiSwapChain* swapchain) {

	if (QRenderSystem::instance()->renderer()) {
		if (mRhi->beginFrame(swapchain) == QRhi::FrameOpSuccess) {
			swapchain->currentFrameRenderTarget()->setRenderPassDescriptor(swapchain->renderPassDescriptor());
			QRenderSystem::instance()->renderer()->render(swapchain->currentFrameCommandBuffer());
			mRhi->endFrame(swapchain);
		}
	}
	qint64 current = mTimer.elapsed();
	mDurationMs = current - mLastTimeMs;
	mLastTimeMs = current;
}

void QRenderWindow::customResizeEvent() {
	if (QRenderSystem::instance()->renderer()) {
		QRenderSystem::instance()->renderer()->rebuild();
	}
}
