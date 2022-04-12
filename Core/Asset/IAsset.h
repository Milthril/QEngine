#ifndef IAsset_h__
#define IAsset_h__

#include <QString>

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

protected:
	QString mName;
};


#endif // IAsset_h__
