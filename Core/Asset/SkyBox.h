#ifndef SkyBox_h__
#define SkyBox_h__

namespace Asset {

class SkyBox: public IAsset {
private:
	std::array<QImage, 6> mImage;
};

}

#endif // SkyBox_h__
