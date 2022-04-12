#include "StaticMesh.h"
#include "QMetaType"

void Asset::StaticMesh::serialize(QDataStream& out) {
	out << QMetaTypeId2<Asset::StaticMesh>::qt_metatype_id();
	out << mVertices;
	out << mIndices;
	out
}

void Asset::StaticMesh::deserialize(QDataStream& in) {
	throw std::logic_error("The method or operation is not implemented.");
}

