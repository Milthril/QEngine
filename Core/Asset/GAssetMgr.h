#ifndef GAssetMgr_h__
#define GAssetMgr_h__

#include "Importer\QAssetImporter.h"
#include "IAsset.h"
#include "Utils/StdUUID.h"
#include "Serialization/QSerialization.h"

#define TheAssetMgr (GAssetMgr::Instance())

class GAssetMgr {
public:
	static GAssetMgr* Instance();

	void import(QString filePath, QDir destDir);

	std::shared_ptr<IAsset> load(QString path, IAsset::Type type = IAsset::None);

	template<typename AssetType>
	std::shared_ptr<AssetType> load(QString path) {
		if (QFileInfo(path).isRelative()) {
			path = mAssetDir.filePath(path);
		}
		if (mAssetCache.contains(path)) {
			qInfo() << "Load Asset From Cache:" << path;
			return std::dynamic_pointer_cast<AssetType>(mAssetCache[path]);
		}
		qInfo() << "Load Asset From File:" << path;
		QFile file(path);
		if (file.open(QFile::ReadOnly)) {
			QDataStream in(&file);
			AssetType* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			auto mAsset = std::shared_ptr<AssetType>(asset);
			cacheAsset(path, mAsset);
			return mAsset;
		}
		return nullptr;
	}
	void createNewAsset(QDir destDir,IAsset::Type type = IAsset::None);
	void updateSHA256(QString path, QByteArray sha256);
	void cacheAsset(QString path, std::shared_ptr<IAsset> asset);
	QByteArray getSHA256(QString path);
	const QDir& assetDir() const { return mAssetDir; }

	void shutdown();
private:
	GAssetMgr();
	~GAssetMgr();
private:
	QDir mAssetDir;
	QAssetImpoerter mImporter;
	using AssetPath = QString;
	QHash<AssetPath, QByteArray> mAssetSHA256Cache;;
	QHash<AssetPath, std::shared_ptr<IAsset>> mAssetCache;
};

#endif // GAssetMgr_h__
