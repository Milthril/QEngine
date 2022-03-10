#include "QRhiWindow.h"
#include "QVulkanInstance"
#include <QtGui/private/qrhinull_p.h>
#include "private\qrhid3d11_p.h"
#include "private\qrhivulkan_p.h"
#include "private\qrhimetal_p.h"
#include "private\qrhigles2_p.h"
#include <QPlatformSurfaceEvent>
#include "QGuiApplication"
#include "Render/Renderer/DefaultRenderer/QDefaultRenderer.h"

QRhiWindow::QRhiWindow(QRhi::Implementation backend)
	: mBackend(backend)
	, mScene(new QScene)
{
	switch (backend) {
	case QRhi::OpenGLES2:
		setSurfaceType(OpenGLSurface);
		setFormat(QRhiGles2InitParams::adjustedFormat());
		break;
	case QRhi::Vulkan:
		setSurfaceType(VulkanSurface);
		break;
	case QRhi::D3D11:
		setSurfaceType(Direct3DSurface);
		break;
	case QRhi::Metal:
		setSurfaceType(MetalSurface);
		break;
	default:
		break;
	}
}

void QRhiWindow::setDefaultSurfaceFormat(QSurfaceFormat format)
{
	QSurfaceFormat::setDefaultFormat(format);
}

void QRhiWindow::waitExposed()
{
	while (!mRunning) {
		QGuiApplication::processEvents();
	}
}

void QRhiWindow::initInternal()
{
	QRhi::Flags rhiFlags = QRhi::EnableDebugMarkers | QRhi::EnableProfiling;
	if (mBackend == QRhi::Null) {
		QRhiNullInitParams params;
		mRhi.reset(QRhi::create(QRhi::Null, &params, rhiFlags));
	}
	else if (mBackend == QRhi::OpenGLES2) {
		mFallbackSurface.reset(QRhiGles2InitParams::newFallbackSurface());
		QRhiGles2InitParams params;
		params.fallbackSurface = mFallbackSurface.get();
		params.window = this;
		mRhi.reset(QRhi::create(QRhi::OpenGLES2, &params, rhiFlags));
	}
	else if (mBackend == QRhi::Vulkan) {
		QRhiVulkanInitParams params;
		static QVulkanInstance vkinstance;
		if (!vkinstance.isValid()) {
			vkinstance.setLayers({ "VK_LAYER_KHRONOS_validation" });
			vkinstance.setExtensions(QRhiVulkanInitParams::preferredInstanceExtensions());
			if (!vkinstance.create())
				qFatal("Failed to create Vulkan instance");
		}
		setVulkanInstance(&vkinstance);
		params.inst = vulkanInstance();
		params.window = this;
		mRhi.reset(QRhi::create(QRhi::Vulkan, &params, rhiFlags));
	}
	else  if (mBackend == QRhi::D3D11) {
		QRhiD3D11InitParams params;
		params.enableDebugLayer = true;
		mRhi.reset(QRhi::create(QRhi::D3D11, &params, rhiFlags));
	}
	else if (mBackend == QRhi::Metal) {
		QRhiMetalInitParams params;
		mRhi.reset(QRhi::create(QRhi::Metal, &params, rhiFlags));
	}

	mSwapChain.reset(mRhi->newSwapChain());
	mSwapChain->setWindow(this);
	mSwapChain->setSampleCount(8);
	mDepthStencilFrameBuffer.reset(mRhi->newRenderBuffer(QRhiRenderBuffer::DepthStencil,
								   QSize(), // no need to set the size here, due to UsedWithSwapChainOnly
								   mSwapChain->sampleCount(),
								   QRhiRenderBuffer::UsedWithSwapChainOnly));
	mSwapChain->setDepthStencil(mDepthStencilFrameBuffer.get());
	mRenderPassDesciptor.reset(mSwapChain->newCompatibleRenderPassDescriptor());
	mSwapChain->setRenderPassDescriptor(mRenderPassDesciptor.get());
	resizeSwapChain();

	mRootRenderer = std::make_shared<QDefaultRenderer>(mRhi, mSwapChain->sampleCount(), mRenderPassDesciptor);
	mRootRenderer->setScene(mScene);
}

void QRhiWindow::renderInternal()
{
	if (!isExposed())
		return;
	if (!mHasSwapChain)
		return;
	if (mSwapChain->currentPixelSize() != mSwapChain->surfacePixelSize() || mNewlyExposed) {
		resizeSwapChain();
		if (!mHasSwapChain)
			return;
		mNewlyExposed = false;
	}
	QRhi::FrameOpResult ret = mRhi->beginFrame(mSwapChain.get());
	if (ret == QRhi::FrameOpSwapChainOutOfDate) {
		resizeSwapChain();
		if (!mHasSwapChain)
			return;
		mRhi->beginFrame(mSwapChain.get());;
	}
	if (ret != QRhi::FrameOpSuccess) {
		qDebug("beginFrame failed with %d, retry", ret);
		return;
	}
	if (mRootRenderer) {
		mSwapChain->currentFrameRenderTarget()->setRenderPassDescriptor(mRenderPassDesciptor.get());
		mRootRenderer->renderInternal(mSwapChain->currentFrameCommandBuffer(), mSwapChain->currentFrameRenderTarget());
	}
	mRhi->endFrame(mSwapChain.get());

	mFrameCount += 1;
	if (mTimer.elapsed() > 1000) {
		qDebug("ca. %d fps", mFrameCount);
		mTimer.restart();
		mFrameCount = 0;
	}
}

void QRhiWindow::resizeSwapChain()
{
	mHasSwapChain = mSwapChain->createOrResize();
	mFrameCount = 0;
	mTimer.restart();
}

void QRhiWindow::exposeEvent(QExposeEvent*)
{
	if (isExposed()) {
		if (!mRunning) {
			mRunning = true;
			initInternal();
		}
		resizeSwapChain();
	}

	const QSize surfaceSize = mHasSwapChain ? mSwapChain->surfacePixelSize() : QSize();
	// stop pushing frames when not exposed (or size is 0)
	if ((!isExposed() || (mHasSwapChain && surfaceSize.isEmpty())) && mRunning && !mNotExposed) {
		qDebug("not exposed");
		mNotExposed = true;
	}

	// Continue when exposed again and the surface has a valid size. Note that
	// surfaceSize can be (0, 0) even though size() reports a valid one, hence
	// trusting surfacePixelSize() and not QWindow.
	if (isExposed() && mRunning && mNotExposed && !surfaceSize.isEmpty()) {
		qDebug("exposed again");
		mNotExposed = false;
		mNewlyExposed = true;
	}

	// always render a frame on exposeEvent() (when exposed) in order to update
	// immediately on window resize.
	if (isExposed() && !surfaceSize.isEmpty())
		renderInternal();
}

bool QRhiWindow::event(QEvent* e)
{
	switch (e->type()) {
	case QEvent::UpdateRequest:
		renderInternal();
		break;
	case QEvent::PlatformSurface:
		// this is the proper time to tear down the swapchain (while the native window and surface are still around)
		if (static_cast<QPlatformSurfaceEvent*>(e)->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed) {
			mSwapChain.reset(nullptr);
			mRhi.reset();
			mHasSwapChain = false;
		}

		break;

	default:
		break;
	}
	return QWindow::event(e);
}