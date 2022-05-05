#include "Cube.h"

namespace QPositionGenerator {

void Cube::generate(QVector<Asset::ParticleSystem::Particle>& particles) {
	for (auto& particle : particles) {
		particle.position = QVector3D(mRandom.bounded(mWidth) - mWidth / 2, mRandom.bounded(mHeight) - mHeight / 2, mRandom.bounded(mDepth) - mDepth / 2).toVector4D();
	}
}

}