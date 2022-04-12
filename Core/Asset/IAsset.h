#ifndef IAsset_h__
#define IAsset_h__

#include<QByteArray>

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

	virtual void serialize(QDataStream& out) = 0;
	virtual void deserialize(QDataStream& in) = 0;

	static IAsset* Deserialize(QDataStream& in);
};

#endif // IAsset_h__
