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

	virtual void serialize(QDataStream& out) override;
	virtual void deserialize(QDataStream& in) override;
private:
	QByteArray mShadingCode;
};
}

#endif // Material_h__
