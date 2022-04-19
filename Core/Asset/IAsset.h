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
		SkyBox ,
		StaticMesh ,
		ParticleSystem,
		Skeleton,
		SkeletonModel,
		SkeletonAnimation,
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
		{Skeleton,"QSkeleton"},
		{SkeletonModel,"QSkeletonModel"},
		{SkeletonAnimation,"QSkeletonAnimation"},
	};
	virtual void serialize(QDataStream& out) {}
	virtual void deserialize(QDataStream& in) {}

	QByteArray getSHA();
protected:
	QString mRefPath;
};

#define Q_ENGINE_DECLARE_ASSET_TYPE(AssetType,EnumType) \
	Q_DECLARE_BUILTIN_METATYPE(AssetType, EnumType, AssetType); \
	Q_DECLARE_METATYPE(std::shared_ptr<AssetType>);

#endif // IAsset_h__
