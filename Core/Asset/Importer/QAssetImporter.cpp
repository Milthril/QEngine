#include "QAssetImporter.h"
#include "ImporterModelTask.h"
#include "ImporterImageTask.h"

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

	if (!QFile::exists(path))
		return;
	QFileInfo info(path);
	QString suffix = info.suffix();

	std::shared_ptr<ImporterTask> task;

	if (suffix.compare(("FBX"), Qt::CaseInsensitive) == 0
		|| suffix.compare(("MMD"), Qt::CaseInsensitive) == 0
		|| suffix.compare(("OBJ"), Qt::CaseInsensitive) == 0
		|| suffix.compare(("PMX"), Qt::CaseInsensitive) == 0
		|| suffix.compare(("GLB"), Qt::CaseInsensitive) == 0
		) {
		task = std::make_shared<ImporterModelTask>();
	}
	else if (suffix.compare(("PNG"), Qt::CaseInsensitive) == 0
			 || suffix.compare(("JPG"), Qt::CaseInsensitive) == 0
			 || suffix.compare(("JPEG"), Qt::CaseInsensitive) == 0
			 || suffix.compare(("TGA"), Qt::CaseInsensitive) == 0
			 ) {
		task = std::make_shared<ImporterImageTask>();
	}
	task->setDestDir(destDir);
	task->setFilePath(path);

	mTaskList << task;
	QMetaObject::invokeMethod(this, &QAssetImpoerter::threadFunc);
}

void QAssetImpoerter::threadFunc() {
	while (!mTaskList.isEmpty()) {
		mTaskList.takeFirst()->executable();
	}
}

