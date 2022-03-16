#include "QMaterial.h"

QMaterial::QMaterial()
{
	mShadingCode = "FragColor = vec4(1);";
	bNeedRecreate = true;
}

void QMaterial::addRef(QPrimitiveComponent* comp)
{
	mRef << comp;
}

void QMaterial::removeRef(QPrimitiveComponent* comp)
{
	mRef.removeOne(comp);
}