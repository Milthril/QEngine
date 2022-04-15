#include "StaticMesh.h"
#include "QMetaType"
#include "Serialization/QSerialization.h"

namespace Asset {
QDataStream& operator<<(QDataStream& out, const StaticMesh& var) {
	out << var.mName;
	out << var.mIndices; 
	out << var.mVertices;
	out << var.mMaterialPath;
	return out;
}

QDataStream& operator>>(QDataStream& in, StaticMesh& var) {
	in >> var.mName;
	in >> var.mIndices;
	in >> var.mVertices;
	in >> var.mMaterialPath;
	return in;
}

}
