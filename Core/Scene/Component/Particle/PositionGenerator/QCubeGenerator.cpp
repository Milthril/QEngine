#include "QCubeGenerator.h"

void QCubeGenerator::generate(QVector<QParticleSystem::Particle>& particles)
{
	for (auto& particle : particles) {
		particle.position = QVector3D(mRandom.bounded(mWidth) - mWidth / 2, mRandom.bounded(mHeight) - mHeight / 2, mRandom.bounded(mDepth) - mDepth / 2).toVector4D();
	}
}
