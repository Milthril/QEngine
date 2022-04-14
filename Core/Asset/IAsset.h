#ifndef IAsset_h__
#define IAsset_h__

#include <QString>
#include <QMetaType>
#include "QMap"
#include "QFile"

class IAsset {
public:
	enum Type {
		None = 0,
		Material,
		SkyBox,
		StaticMesh,
		ParticleSystem
	};
	virtual IAsset::Type type() = 0;

	QString getName() const { return mName; }
	void setName(QString val) { mName = val; }

	QString getExtName() {
		return mAssetExtName[type()];
	}

	static std::shared_ptr<IAsset> CreateAssetFromPath(QString path, IAsset::Type type = None);

	template<typename AssetType>
	static std::shared_ptr<AssetType> LoadAsset(QString path);

	inline static QMap<Type, QString> mAssetExtName = {
		{Material,"QMaterial"},
		{SkyBox,"QSkyBox"},
		{StaticMesh,"QStaticMesh"},
		{ParticleSystem,"QParticleSystem"},
	};
protected:
	QString mName;
};

#endif // IAsset_h__
