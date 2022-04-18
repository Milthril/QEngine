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
}

void QRenderWindow::customRenderEvent(QRhiSwapChain* swapchain) {
	if (QRenderSystem::instance()->renderer()) {
		if (mRhi->beginFrame(swapchain) == QRhi::FrameOpSuccess) {
			swapchain->currentFrameRenderTarget()->setRenderPassDescriptor(swapchain->renderPassDescriptor());
			QRenderSystem::instance()->renderer()->render(swapchain->currentFrameCommandBuffer());
			mRhi->endFrame(swapchain);
		}
	}
}

void QRenderWindow::customResizeEvent() {
	if (QRenderSystem::instance()->renderer()) {
		QRenderSystem::instance()->renderer()->rebuild();
	}
}
