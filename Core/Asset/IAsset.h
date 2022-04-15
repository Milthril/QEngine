#ifndef IAsset_h__
#define IAsset_h__

#include <QString>
#include <QMetaType>
#include "QMap"
#include "QFile"
#include "QFileInfo"

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

	QString getName() const { return QFileInfo(mRefPath).baseName(); }
	QString getRefPath() const { return mRefPath; }
	void setRefPath(QString val) { mRefPath = val; }

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
	QString mRefPath;

};

#endif // IAsset_h__
