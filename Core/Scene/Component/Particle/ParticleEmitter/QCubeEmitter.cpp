#include "QCubeEmitter.h"

void QCubeEmitter::handParticle(Particle& particle)
{
	particle.position = QVector3D(mRandom.bounded(mWidth) - mWidth / 2, mRandom.bounded(mHeight) - mHeight / 2, mRandom.bounded(mDepth) - mDepth / 2).toVector4D();
}