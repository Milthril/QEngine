#include "Material.h"
#include "Serialization\QSerialization.h"



namespace Asset{

void Material::setShadingCode(QByteArray val) {
	mShadingCode = val;

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

