#include "StaticMesh.h"
#include "QMetaType"

namespace Asset {
QDataStream& operator<<(QDataStream& out, const StaticMesh& var) {
	out << var.mName;
	out << var.mIndices; 
	out << var.mMaterial;
	out << var.mVertices;
	return out;
}

QDataStream& operator>>(QDataStream& in, StaticMesh& var) {
	in >> var.mName;
	in >> var.mIndices;
	in >> var.mMaterial;
	in >> var.mVertices;
	return in;
}

}
