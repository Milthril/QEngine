#include "QEngine.h"
#include "EventHandler\QTickEventHandler.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"


QEngine::QEngine(int argc, char** argv, bool enableDebug /*= false*/)
	: QApplication(argc, argv)
	, mWorld(std::make_shared<QWorld>())
{
	if (QDir(QENGINE_ASSET_DIR).exists()) {
		mAssetDir = QDir(QENGINE_ASSET_DIR);
	}
}

void QEngine::customInit() {

}

void QEngine::customUpdate()
{
}

const std::shared_ptr<QWorld>& QEngine::world()
{
	return mWorld;
}



void QEngine::execGame()
{
	customInit();
	QRenderSystem::instance()->init();

	mLastTime = mTimer.elapsed();
	int64_t newTime;
	float deltaSeconds;

	while (!QRenderSystem::instance()->hasRequestQuit()) {
		newTime = mTimer.elapsed();
		deltaSeconds = (newTime - mLastTime) / 1000.0f;
		mLastTime = newTime;
		mTimer.elapsed();
		customUpdate();
		QGuiApplication::processEvents();
		QTickEventHandler::processEvent(deltaSeconds);
		QRenderSystem::instance()->requestUpdate();
	}
	QRenderSystem::instance()->shutdown();
}
