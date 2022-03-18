#include "QCubeGenerator.h"

QVector3D QCubeGenerator::generate()
{
	return	QVector3D(mRandom.bounded(mWidth) - mWidth / 2, mRandom.bounded(mHeight) - mHeight / 2, mRandom.bounded(mDepth) - mDepth / 2);
}