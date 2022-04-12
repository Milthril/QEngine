#ifndef IAsset_h__
#define IAsset_h__

#include "QObject"

class IAsset:public QObject {
public:
	enum Type {
		None = 0,
		Material,
		SkyBox,
		StaticMesh,
		ParticleSystem
	};
	virtual IAsset::Type type() = 0;
};


#endif // IAsset_h__
