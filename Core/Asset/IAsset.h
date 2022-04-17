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
		None = 99999,
		Material = 100000,
		SkyBox = 100001,
		StaticMesh = 100002,
		ParticleSystem = 100003,
		SkeletonModel = 100004,
		SkeletonAnimation = 100005,
	};

	virtual IAsset::Type type() = 0;

	QString getName() const;
	QString getRefPath() const;
	QString getExtName();

	void setRefPath(QString val);
	virtual void save(QString filePath,bool cover = true);

	inline static QMap<Type, QString> AssetExtName = {
		{Material,"QMaterial"},
		{SkyBox,"QSkyBox"},
		{StaticMesh,"QStaticMesh"},
		{ParticleSystem,"QParticleSystem"},
	};
	virtual void serialize(QDataStream& out) {}
	virtual void deserialize(QDataStream& in) {}
protected:
	QString mRefPath;
};

#endif // IAsset_h__
