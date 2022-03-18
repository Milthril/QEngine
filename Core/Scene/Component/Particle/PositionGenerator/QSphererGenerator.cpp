#include "QSphererGenerator.h"

QVector3D QSphereGenerator::generate()
{
	return	QVector3D(mRandom.bounded(mWidth) - mWidth / 2, mRandom.bounded(mHeight) - mHeight / 2, mRandom.bounded(mDepth) - mDepth / 2);
}