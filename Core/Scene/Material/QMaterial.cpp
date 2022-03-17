#include "QMaterial.h"

QMaterial::QMaterial()
{
	mShadingCode = "FragColor = vec4(1);";
}

void QMaterial::setShadingCode(QByteArray val)
{
	if (val != mShadingCode) {
		mShadingCode = val;
		bNeedRecreate.active();
	}
}