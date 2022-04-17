#include "GAssetMgr.h"

#include "Material.h"
#include "SkyBox.h"
#include "StaticMesh.h"

#include "Serialization/QSerialization.h"
#include "PartcleSystem/ParticleSystem.h"
#include "Utils/FileUtils.h"


GAssetMgr* GAssetMgr::Instance() {
	static GAssetMgr ins;
	return &ins;
}

void GAssetMgr::import(QString filePath, QDir destDir) {
	mImporter.import(filePath, destDir);
}

std::shared_ptr<IAsset> GAssetMgr::load(QString path, IAsset::Type type /*= IAsset::None*/) {
	if (type == IAsset::None) {
		if (path.endsWith(IAsset::AssetExtName[IAsset::Material])) 
			type = IAsset::Material;
		else if (path.endsWith(IAsset::AssetExtName[IAsset::SkyBox])) 
			type = IAsset::SkyBox;
		else if (path.endsWith(IAsset::AssetExtName[IAsset::StaticMesh])) 
			type = IAsset::StaticMesh;
		else if (path.endsWith(IAsset::AssetExtName[IAsset::ParticleSystem])) 
			type = IAsset::ParticleSystem;
		
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
						   break;
		default:
			return nullptr;
		}
	}
	return nullptr;
}

template<typename AssetType>
std::shared_ptr<AssetType>
GAssetMgr::load(QString path) {
	QFile file(path);
	if (file.open(QFile::ReadOnly)) {
		QDataStream in(&file);
		AssetType* asset = nullptr;
		in >> asset;
		asset->setRefPath(path);
		return std::shared_ptr<AssetType>(asset);
	}
	return nullptr;
}

template std::shared_ptr<Asset::Material> GAssetMgr::load<>(QString path);
template std::shared_ptr<Asset::StaticMesh> GAssetMgr::load<>(QString path);
template std::shared_ptr<Asset::SkyBox> GAssetMgr::load<>(QString path);
template std::shared_ptr<Asset::ParticleSystem> GAssetMgr::load<>(QString path);

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
