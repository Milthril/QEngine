#include "QMaterial.h"

QMaterial::QMaterial()
{
	mShadingCode = "FragColor = vec4(1);";
	bNeedRecreate = true;
}