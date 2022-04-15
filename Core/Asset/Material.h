#ifndef Material_h__
#define Material_h__

#include "IAsset.h"
#include "ECS\System\RenderSystem\RHI\QRhiUniform.h"

namespace Asset {

class Material:public IAsset,public QRhiUniform {

public:
	virtual IAsset::Type type() override {
		return IAsset::Material;
	}
	QByteArray getShadingCode() const { return mShadingCode; }
	void setShadingCode(QByteArray val);

	friend QDataStream& operator<<(QDataStream& out, const Material& var);
	friend QDataStream& operator>>(QDataStream& in, Material& var);
private:
	QByteArray mShadingCode = "FragColor = vec4(1);";
};
}
//Q_DECLARE_METATYPE(Asset::Material);
Q_DECLARE_BUILTIN_METATYPE(Asset::Material, 100000, Asset::Material);

#endif // Material_h__
