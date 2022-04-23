#include "StaticMesh.h"
#include "QMetaType"
#include "Serialization/QSerialization.h"

namespace Asset {
void StaticMesh::serialize(QDataStream& out) {
	IAsset::serialize(out);
	out << mIndices;
	out << mVertices;
	out << mMaterialPath;
}

void StaticMesh::deserialize(QDataStream& in) {
	IAsset::deserialize(in);
	in >> mIndices;
	in >> mVertices;
	in >> mMaterialPath;
}

}
