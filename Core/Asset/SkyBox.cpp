#include "SkyBox.h"

namespace Asset {

QDataStream& operator<<(QDataStream& out, const SkyBox& var) {
	out <<var.mName
		<<var.mImageList[0]
		<< var.mImageList[1]
		<< var.mImageList[2]
		<< var.mImageList[3]
		<< var.mImageList[4]
		<< var.mImageList[5];
	auto it = var.mImageList[5].format();
	return out;
}

QDataStream& operator>>(QDataStream& in, SkyBox& var) {
	in  >> var.mName
		>> var.mImageList[0]
		>> var.mImageList[1]
		>> var.mImageList[2]
		>> var.mImageList[3]
		>> var.mImageList[4]
		>> var.mImageList[5];
	for (int i = 0; i < std::size(var.mImageList); i++) {
		var.mImageList[i]= var.mImageList[i].convertToFormat(QImage::Format::Format_RGBA8888);
	}
	return in;
}

}

