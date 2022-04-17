#ifndef GAssetMgr_h__
#define GAssetMgr_h__

#include "Importer\QAssetImporter.h"
#include "IAsset.h"
#include "Utils/StdUUID.h"

#define TheAssetMgr (GAssetMgr::Instance())

class GAssetMgr {
public:
	static GAssetMgr* Instance();

	void import(QString filePath, QDir destDir);

	std::shared_ptr<IAsset> load(QString path, IAsset::Type type = IAsset::None);

	template<typename AssetType>
	std::shared_ptr<AssetType> load(QString path);

	void createNewAsset(QDir destDir,IAsset::Type type = IAsset::None);

	void updateSHA256(QString path, QByteArray sha256);

	QByteArray getSHA256(QString path);
private:
	GAssetMgr();
	~GAssetMgr();
private:
	QAssetImpoerter mImporter;
	using AssetPath = QString;
	QHash<AssetPath, QByteArray> mAssetSHA256Cache;;
};

#endif // GAssetMgr_h__
