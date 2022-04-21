#include "QEngineCoreApplication.h"
#include "EventHandler\QTickEventHandler.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "Asset\GAssetMgr.h"


QEngineCoreApplication::QEngineCoreApplication(int argc, char** argv, bool enableDebug /*= false*/)
	: QApplication(argc, argv)
	, mWorld(std::make_shared<QWorld>())
{
	TheRenderSystem->setEnableDebug(enableDebug);
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
	TheRenderSystem->init();
	customInit();

	mLastTime = mTimer.elapsed();

	while (!TheRenderSystem->hasRequestQuit()) {
		processEvents();
	}
	mWorld.reset();
	customRelease();
	TheAssetMgr->shutdown();
	TheRenderSystem->shutdown();
}

void QEngineCoreApplication::processEvents() {
	int64_t newTime = mTimer.elapsed();
	float deltaSeconds = (newTime - mLastTime) / 1000.0f;
	mLastTime = newTime;
	mTimer.elapsed();
	customUpdate();
	QGuiApplication::processEvents();
	QTickEventHandler::processEvent(deltaSeconds);
	TheRenderSystem->requestUpdate();
}
