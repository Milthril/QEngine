#include "QEngine.h"
#include "Renderer\CommonPass\DebugDrawPass.h"
#include "EventHandler\QTickEventHandler.h"

QEngine::QEngine(int argc, char** argv, bool enableDebug /*= false*/)
	: QApplication(argc, argv)
	, mScene(std::make_shared<QScene>())
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::Vulkan))
	, mRenderer(std::make_shared<QDefaultRenderer>())
{
	mRenderer->setScene(mScene);
	if (enableDebug) {
		mDebugPainter = std::make_shared<DebugDrawPass>();
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

const std::shared_ptr<DebugDrawPass>& QEngine::debugPainter()
{
	return mDebugPainter;
}

void QEngine::execGame()
{
	mWindow->resize(800, 600);
	mWindow->show();
	mWindow->waitExposed();
	init();
	while (!mWindow->mHasClosed) {
		update();
		mWindow->requestUpdate();
		QGuiApplication::processEvents();
		QTickEventHandler::processEvent();
	}
}

QRhi* QEngine::getRHI()
{
	return mWindow->mRhi.get();
}