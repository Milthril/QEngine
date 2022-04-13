#ifndef SkyBox_h__
#define SkyBox_h__

#include "IAsset.h"
#include <array>
#include <QImage>

namespace Asset {

class SkyBox: public IAsset {

public:
	friend QDataStream& operator<<(QDataStream& out, const SkyBox& var);
	friend QDataStream& operator>>(QDataStream& in, SkyBox& var);

	const std::array<QImage, 6>& getImageList() const { return mImageList; }
	void setImageList(std::array<QImage, 6> val) { mImageList = val; }

	virtual IAsset::Type type() override {
		return IAsset::SkyBox;
	}

private:
	std::array<QImage, 6> mImageList;
};
}

#endif // SkyBox_h__
