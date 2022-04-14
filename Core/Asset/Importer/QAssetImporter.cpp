#include "QAssetImporter.h"
#include "QApplication"
#include "Serialization\QSerialization.h"
#include "Asset\Material.h"
#include "Asset\StaticMesh.h"
#include "Asset\SkyBox.h"

QAssetImpoerter* QAssetImpoerter::instance() {
	static QAssetImpoerter ins;
	return &ins;
}

QAssetImpoerter::~QAssetImpoerter() {
	if (mThread.isRunning()) {
		mThread.quit();
		mThread.wait();
	}
}

void QAssetImpoerter::import(QString path, QDir destDir) {
	auto task = std::make_shared<ImporterTask>(path, destDir);
	mTaskList << task;
	QMetaObject::invokeMethod(this, &QAssetImpoerter::threadFunc);
}


QAssetImpoerter::QAssetImpoerter() {
	moveToThread(&mThread);
	mThread.start();
}

void QAssetImpoerter::threadFunc() {
	while (!mTaskList.isEmpty()) {
		auto task = mTaskList.takeFirst();
		task->executable();
	}
}

