#include "QEngine.h"

QEngine::QEngine(int argc, char** argv)
	: QApplication(argc, argv)
	, mScene(std::make_shared<QScene>())
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::Vulkan))
	, mRenderer(std::make_shared<QDefaultRenderer>())
{
	mRenderer->setScene(mScene);
}

void QEngine::init()
{
}

void QEngine::update()
{
}

std::shared_ptr<QScene> QEngine::scene()
{
	return mScene;
}

std::shared_ptr<QRhiWindow> QEngine::window()
{
	return mWindow;
}

std::shared_ptr<QSceneRenderer> QEngine::renderer()
{
	return mRenderer;
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