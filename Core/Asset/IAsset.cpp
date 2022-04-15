#include "IAsset.h"
#include "QFile"
#include "Material.h"
#include "SkyBox.h"
#include "StaticMesh.h"

#include "Serialization/QSerialization.h"
#include "PartcleSystem/ParticleSystem.h"

template<typename AssetType>
static std::shared_ptr<AssetType>
IAsset::LoadAsset(QString path) {
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

template std::shared_ptr<Asset::Material> IAsset::LoadAsset<>(QString path);
template std::shared_ptr<Asset::StaticMesh> IAsset::LoadAsset<>(QString path);
template std::shared_ptr<Asset::SkyBox> IAsset::LoadAsset<>(QString path);

std::shared_ptr<IAsset> IAsset::CreateAssetFromPath(QString path, Type type /*= None*/) {
	if (type == None) {
		if (path.endsWith(mAssetExtName[Material])) {
			type = Material;
		}
		else if (path.endsWith(mAssetExtName[SkyBox])) {
			type = SkyBox;
		}
		else if (path.endsWith(mAssetExtName[StaticMesh])) {
			type = StaticMesh;
		}
		else if (path.endsWith(mAssetExtName[ParticleSystem])) {
			type = ParticleSystem;
		}
	}
	QFile file(path);
	if (file.open(QFile::ReadOnly)) {
		QDataStream in(&file);
		switch (type) {
		case Material: {
			Asset::Material* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		case SkyBox: {
			Asset::SkyBox* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		case StaticMesh:{
			Asset::StaticMesh* asset = nullptr;
			in >> asset;
			asset->setRefPath(path);
			return std::shared_ptr<IAsset>(asset);
		}
		case ParticleSystem: {
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

