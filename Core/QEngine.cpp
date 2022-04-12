#include "QEngine.h"
#include "EventHandler\QTickEventHandler.h"


QEngine::QEngine(int argc, char** argv, bool enableDebug /*= false*/)
	: QApplication(argc, argv)
	, mWorld(std::make_shared<QWorld>())

{
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

const std::shared_ptr<QWorld>& QEngine::world()
{
	return mWorld;
}



void QEngine::execGame()
{
	init();
	mLastTime = mTimer.elapsed();
	int64_t newTime;
	float deltaSeconds;
	//while (!mWindow->mHasClosed) {
	//	newTime = mTimer.elapsed();
	//	deltaSeconds = (newTime - mLastTime)/1000.0f;
	//	mLastTime = newTime;
	//	mTimer.elapsed();
	//	customUpdate();
	//	QGuiApplication::processEvents();
	//	QTickEventHandler::processEvent(deltaSeconds);
	//	mWindow->requestUpdate();
	//}
}
