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
	void setShadingCode(QByteArray val) { mShadingCode = val; }

	friend QDataStream& operator<<(QDataStream& out, const Material& var);
	friend QDataStream& operator>>(QDataStream& in, Material& var);
private:
	QByteArray mShadingCode;
};
}

Q_DECLARE_METATYPE(Asset::Material);

#endif // Material_h__
