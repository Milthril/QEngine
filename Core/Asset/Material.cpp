#include "Material.h"
#include "Serialization\QSerialization.h"



namespace Asset{


Material::Material() {
}

void Material::setShadingCode(QByteArray val) {
	mShadingCode = val;
	bNeedRecreate.active();
}

void Material::generateDefualtShadingCode() {
	if (mParamNameMap.contains("Diffuse")) {
		mShadingCode = "outBaseColor = texture(Diffuse,vUV); \n";
	}
	else {
		setDataVec4("BaseColor", QVector4D(1, 1, 1, 1));
		mShadingCode = "outBaseColor = UBO.BaseColor; \n";
	}
	setDataFloat("Metalness", 1.0);
	setDataFloat("Roughness", 1.0);
	mShadingCode += "outMetalness = UBO.Metalness; \n ";
	mShadingCode += "outRoughness = UBO.Roughness; \n ";
	bNeedRecreate.active();
}

void Material::serialize(QDataStream& out) {
	IAsset::serialize(out);
	QRhiUniform::serialize(out);
	out << mShadingCode;
}

void Material::deserialize(QDataStream& in) {
	IAsset::deserialize(in);
	QRhiUniform::deserialize(in);
	in >> mShadingCode;
}

}

