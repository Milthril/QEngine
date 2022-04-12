#include "QAssetImporter.h"

QAssetImpoerter* QAssetImpoerter::instance() {
	static QAssetImpoerter ins;
	return &ins;
}

void QAssetImpoerter::import(QString path, QDir destDir) {
	auto task = std::make_shared<ImporterTask>(path, destDir);
	mTaskList << task;
	if (!mThread.isRunning()) {
		mThread.start();
		QMetaObject::invokeMethod(this, &QAssetImpoerter::threadFunc);
	}
}

QAssetImpoerter::QAssetImpoerter() {
	moveToThread(&mThread);
}

void QAssetImpoerter::threadFunc() {
	while (!mTaskList.isEmpty()) {
		auto task = mTaskList.takeFirst();
		task->executable();
	}
	mThread.quit();
}

