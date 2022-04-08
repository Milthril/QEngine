#include "QEngine.h"
#include "EventHandler\QTickEventHandler.h"

QEngine::QEngine(int argc, char** argv, bool enableDebug /*= false*/)
	: QApplication(argc, argv)
	, mScene(std::make_shared<QScene>())
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::Vulkan))
	, mRenderer(std::make_shared<QDefaultRenderer>())
{
	mRenderer->setScene(mScene);
	mRenderer->setEnableDebug(enableDebug);
	if (QDir(QENGINE_ASSET_DIR).exists()) {
		mAssetDir = QDir(QENGINE_ASSET_DIR);
	}
}

void QEngine::init()
{
}

void QEngine::customUpdate()
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

const std::shared_ptr<ISceneRenderer>& QEngine::renderer()
{
	return mRenderer;
}

void QEngine::execGame()
{
	mWindow->resize(800, 600);
	mWindow->show();
	mWindow->waitExposed();
	init();
	mTimer.start();
	mLastTime = mTimer.elapsed();
	int64_t newTime;
	float deltaSeconds;
	while (!mWindow->mHasClosed) {
		newTime = mTimer.elapsed();
		deltaSeconds = (newTime - mLastTime)/1000.0f;
		mLastTime = newTime;
		mTimer.elapsed();
		customUpdate();
		QGuiApplication::processEvents();
		QTickEventHandler::processEvent(deltaSeconds);
		mWindow->requestUpdate();
	}
}

QRhi* QEngine::getRHI()
{
	return mWindow->mRhi.get();
}