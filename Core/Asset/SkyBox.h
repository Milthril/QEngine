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

Q_ENGINE_DECLARE_ASSET_TYPE(Asset::SkyBox, IAsset::SkyBox);

#endif // SkyBox_h__
