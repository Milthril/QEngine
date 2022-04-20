#include "QEngineCoreApplication.h"
#include "EventHandler\QTickEventHandler.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"


QEngineCoreApplication::QEngineCoreApplication(int argc, char** argv, bool enableDebug /*= false*/)
	: QApplication(argc, argv)
	, mWorld(std::make_shared<QWorld>())
{
	if (QDir(QEngineCoreApplication_ASSET_DIR).exists()) {
		mAssetDir = QDir(QEngineCoreApplication_ASSET_DIR);
	}
	QRenderSystem::instance()->setEnableDebug(enableDebug);
}

void QEngineCoreApplication::customInit() {

}

void QEngineCoreApplication::customUpdate()
{
}

void QEngineCoreApplication::customRelease() {

}

const std::shared_ptr<QWorld>& QEngineCoreApplication::world()
{
	return mWorld;
}

void QEngineCoreApplication::execGame()
{
	QRenderSystem::instance()->init();
	customInit();

	mLastTime = mTimer.elapsed();

	while (!QRenderSystem::instance()->hasRequestQuit()) {
		processEvents();
	}
	mWorld.reset();
	customRelease();
	QRenderSystem::instance()->shutdown();
}

void QEngineCoreApplication::processEvents() {
	int64_t newTime = mTimer.elapsed();
	float deltaSeconds = (newTime - mLastTime) / 1000.0f;
	mLastTime = newTime;
	mTimer.elapsed();
	customUpdate();
	QGuiApplication::processEvents();
	QTickEventHandler::processEvent(deltaSeconds);
	QRenderSystem::instance()->requestUpdate();
}
