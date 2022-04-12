#include "Material.h"

void Asset::Material::serialize(QDataStream& out) {
	out << QMetaTypeId2<Asset::Material>::qt_metatype_id();
	out << mShadingCode;
}

void Asset::Material::deserialize(QDataStream& in) {
	in >> mShadingCode;
}
