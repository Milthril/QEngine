
#include "QAroundGenerator.h"

void QAroundGenerator::generate(QVector<Asset::ParticleSystem::Particle>& particles) {
	for (auto& particle : particles) {
		float a = mRandom.bounded(M_PI);
		float b = mRandom.bounded(M_PI * 2);
		particle.velocity = getStrength() * QVector3D(qSin(a) * qCos(b), qSin(a) * qSin(b), qCos(a));
	}
}
