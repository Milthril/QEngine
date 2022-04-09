
#include "QScenePrimitiveGenerator.h"

void QScenePrimitiveGenerator::generate(QVector<QParticleSystem::Particle>& particles) {
	if (mGenFunctor)
		mGenFunctor(particles);
}
