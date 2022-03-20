#include "QEngine.h"
#include "Renderer\Common\QDebugPainter.h"

QEngine::QEngine(int argc, char** argv,bool enableDebug /*= false*/)
	: QApplication(argc, argv)
	, mScene(std::make_shared<QScene>())
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::Vulkan))
	, mRenderer(std::make_shared<QDefaultRenderer>())
{
	mRenderer->setScene(mScene);
	if (enableDebug) {
		mDebugPainter = std::make_shared<QDebugPainter>();
		mDebugPainter->setupWindow(mWindow.get());
		renderer()->setDegbuPainter(mDebugPainter);
	}
}


void QEngine::init()
{
}

void QEngine::update()
{
}

const std::shared_ptr<QScene>& QEngine::scene()
{
	return mScene;
}

const std::shared_ptr<QRhiWindow>& QEngine::window()
{
	return mWindow;
}

const std::shared_ptr<QSceneRenderer>& QEngine::renderer()
{
	return mRenderer;
}

const std::shared_ptr<QDebugPainter>& QEngine::debugPainter() 
{
	return mDebugPainter;
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
