#include "QAssetImporter.h"
#include "Serialization\QSerialization.h"
#include "..\Material.h"
#include "..\StaticMesh.h"

template<typename AssetType>
std::shared_ptr<AssetType> QAssetImpoerter::load(QString path) {
	if (!path.endsWith(".QAsset"))
		return nullptr;
	QFile file(path);
	if (file.open(QFile::ReadOnly)) {
		QDataStream in(&file);
		AssetType* asset = nullptr;
		in >> asset;
		return std::shared_ptr<AssetType>(asset);
	}
	return nullptr;
}

template std::shared_ptr<Asset::Material> QAssetImpoerter::load<>(QString path);
template std::shared_ptr<Asset::StaticMesh> QAssetImpoerter::load<>(QString path);


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

