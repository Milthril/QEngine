#ifndef SkyBox_h__
#define SkyBox_h__

#include "IAsset.h"
#include <array>
#include <QImage>

namespace Asset {

class SkyBox: public IAsset {

public:
	const std::array<QImage, 6>& getImageList() const { return mImageList; }
	void setImageList(std::array<QImage, 6> val) { mImageList = val; }

	virtual IAsset::Type type() override {
		return IAsset::SkyBox;
	}

	void serialize(QDataStream& out) override;
	void deserialize(QDataStream& in) override;

private:
	std::array<QImage, 6> mImageList;
};
}

Q_DECLARE_BUILTIN_METATYPE(SkyBox, IAsset::SkyBox, Asset::SkyBox);

#endif // SkyBox_h__
