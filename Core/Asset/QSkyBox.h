#ifndef SkyBox_h__
#define SkyBox_h__

#include "IAsset.h"

namespace Asset {

class QSkyBox: public IAsset {
private:
	std::array<QImage, 6> mImage;
};

}

#endif // SkyBox_h__
