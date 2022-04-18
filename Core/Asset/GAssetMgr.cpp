#include "GAssetMgr.h"

#include "Material.h"
#include "SkyBox.h"
#include "StaticMesh.h"
#include "PartcleSystem/ParticleSystem.h"
#include "Utils/FileUtils.h"
#include "SkeletonModel/Skeleton.h"
#include "SkeletonModel/SkeletonModel.h"


GAssetMgr* GAssetMgr::Instance() {
	static GAssetMgr ins;
	return &ins;
}

void GAssetMgr::import(QString filePath, QDir destDir) {
	mImporter.import(filePath, destDir);
}

std::shared_ptr<IAsset> GAssetMgr::load(QString path, IAsset::Type type /*= IAsset::None*/) {
	if (type == IAsset::None) {
		for (auto& assetType : IAsset::AssetExtName.keys()) {
			if (path.endsWith(IAsset::AssetExtName[assetType])) {
				type = assetType;
				break;
			}
		}
	}
	QFile file(path);
	if (file.open(QFile::ReadOnly)) {
		QDataStream in(&file);
		switch (type) {
		case IAsset::Material: {
			Asset::Material* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		case IAsset::SkyBox: {
			Asset::SkyBox* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		case IAsset::StaticMesh: {
			Asset::StaticMesh* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		case IAsset::ParticleSystem: {
			Asset::ParticleSystem* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		case IAsset::Skeleton: {
			Asset::Skeleton* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		case IAsset::SkeletonModel: {
			Asset::SkeletonModel* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		default:
			return nullptr;
		}
	}
	return nullptr;
}

void GAssetMgr::createNewAsset(QDir destDir, IAsset::Type type /*= IAsset::None*/) {
	switch (type) {
	case IAsset::None:
		break;
	case IAsset::Material: {
		std::shared_ptr<Asset::Material> material = std::make_shared<Asset::Material>();
		material->save(destDir.filePath("Material." + material->getExtName()), false);
		break;
	}
	case IAsset::SkyBox:
		break;
	case IAsset::StaticMesh:
		break;
	case IAsset::ParticleSystem:{
		std::shared_ptr<Asset::ParticleSystem> particleSystem = std::make_shared<Asset::ParticleSystem>();
		particleSystem->save(destDir.filePath("PartileSystem." + particleSystem->getExtName()), false);
		break;
	}
	case IAsset::SkeletonModel:
		break;
	case IAsset::SkeletonAnimation:
		break;
	default:
		break;
	}
}

void GAssetMgr::updateSHA256(QString path, QByteArray sha256) {
	Q_ASSERT(!path.isEmpty() ||!sha256.isEmpty());
	mAssetSHA256Cache[path] = sha256;
}

QByteArray GAssetMgr::getSHA256(QString path) {
	auto Iter = mAssetSHA256Cache.find(path);
	if (Iter != mAssetSHA256Cache.end()) {
		return Iter.value();
	}
	return {};
}

GAssetMgr::GAssetMgr() {
	QFile file("Assets.SHA256");
	if (file.open(QFile::ReadOnly)) {
		QDataStream stream(&file);
		stream >> mAssetSHA256Cache;
	}
}

GAssetMgr::~GAssetMgr() {
	QFile file("Assets.SHA256");
	if (file.open(QFile::WriteOnly)) {
		QDataStream stream(&file);
		stream << mAssetSHA256Cache;
	}
}
