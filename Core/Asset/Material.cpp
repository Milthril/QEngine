#include "Material.h"

namespace Asset{

QDataStream& operator<<(QDataStream& out, const Material& var) {
	var.save(out);
	out << var.mName;
	out << var.mShadingCode;
	return out;
}

QDataStream& operator>>(QDataStream& in, Material& var) {
	var.read(in);
	in >> var.mName;
	in >> var.mShadingCode;
	return in;
}
}

