#include "QAssetImporter.h"
#include "QApplication"
#include "Serialization\QSerialization.h"
#include "Asset\Material.h"
#include "Asset\StaticMesh.h"
#include "Asset\SkyBox.h"

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
template std::shared_ptr<Asset::SkyBox> QAssetImpoerter::load<>(QString path);


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

