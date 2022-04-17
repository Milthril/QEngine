#include "QAssetImporter.h"

QAssetImpoerter::QAssetImpoerter() {
	moveToThread(&mThread);
	mThread.start();
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

void QAssetImpoerter::threadFunc() {
	while (!mTaskList.isEmpty()) {
		auto task = mTaskList.takeFirst();
		task->executable();
	}
}

