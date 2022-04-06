#include "QMaterial.h"

QMaterial::QMaterial()
{
	mShadingCode = "FragColor = vec4(0.1,0.5,0.9,1.0);";
}

void QMaterial::setShadingCode(QByteArray val)
{
	if (val != mShadingCode) {
		mShadingCode = val;
		bNeedRecreate.active();
	}
}