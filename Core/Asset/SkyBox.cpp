#include "SkyBox.h"

namespace Asset {

void SkyBox::serialize(QDataStream& out) {
	IAsset::serialize(out);
	out << mImageList[0]
		<< mImageList[1]
		<< mImageList[2]
		<< mImageList[3]
		<< mImageList[4]
		<< mImageList[5];
}

void SkyBox::deserialize(QDataStream& in) {
	IAsset::deserialize(in);
	in >> mImageList[0]
		>> mImageList[1]
		>> mImageList[2]
		>> mImageList[3]
		>> mImageList[4]
		>> mImageList[5];
	for (int i = 0; i < std::size(mImageList); i++) {
		mImageList[i] = mImageList[i].convertToFormat(QImage::Format::Format_RGBA8888);
	}
}

}

