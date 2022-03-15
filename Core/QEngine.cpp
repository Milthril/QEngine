#include "QEngine.h"

QEngine::QEngine(int argc, char** argv)
	: QApplication(argc, argv)
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::Vulkan))
{
}

void QEngine::init()
{
}

void QEngine::update()
{
}

std::shared_ptr<QScene> QEngine::scene()
{
	return mWindow->scene();
}

std::shared_ptr<QRhiWindow> QEngine::window()
{
	return mWindow;
}

void QEngine::execGame()
{
	mWindow->show();
	mWindow->waitExposed();
	init();
	while (true) {
		QGuiApplication::processEvents();
		update();
		mWindow->requestUpdate();
	}
}

QRhi* QEngine::getRHI()
{
	return mWindow->mRhi.get();
}