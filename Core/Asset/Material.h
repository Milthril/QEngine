#ifndef Material_h__
#define Material_h__

#include "IAsset.h"
#include "RHI\QRhiUniform.h"

namespace Asset {

class Material:public IAsset,public QRhiUniform {
public:
	virtual IAsset::Type type() override {
		return IAsset::Material;
	}
	QByteArray mShadingCode;
};
}

#endif // Material_h__
