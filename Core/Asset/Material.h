#ifndef Material_h__
#define Material_h__

#include "IAsset.h"
#include "ECS\System\RenderSystem\RHI\QRhiUniform.h"

namespace Asset {

class Material:public IAsset,public QRhiUniform {
public:
	Material();
	virtual IAsset::Type type() override {
		return IAsset::Material;
	}
	QByteArray getShadingCode() const { return mShadingCode; }
	void setShadingCode(QByteArray val);

	void generateDefualtShadingCode();
	void serialize(QDataStream& out) override;
	void deserialize(QDataStream& in) override;
private:
	QByteArray mShadingCode = "outBaseColor = vec4(1);";
};
}
//Q_DECLARE_METATYPE(Asset::Material);

Q_ENGINE_DECLARE_ASSET_TYPE(Asset::Material, IAsset::Material);

#endif // Material_h__
