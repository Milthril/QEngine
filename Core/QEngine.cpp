#include "QEngine.h"

QEngine::QEngine(int argc, char** argv)
	: QApplication(argc, argv)
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::Vulkan))
{
	mWindow->show();
	mWindow->waitExposed();
}

void QEngine::onGameLoop()
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
	while (true) {
		QGuiApplication::processEvents();
		onGameLoop();
		mWindow->requestUpdate();
	}
}