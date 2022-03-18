#include "QParticleEmitter.h"

QParticleEmitter::QParticleEmitter()
{
	mPositionGenerator.setSubClass("Cube");
}

QVector<QParticleEmitter::Particle> QParticleEmitter::update()
{
	QVector<QParticleEmitter::Particle> particles(qMax(0, mNumOfTick));
	for (QParticleEmitter::Particle& particle : particles) {
		particle.scaling = mScaling.toVector4D();
		particle.position = mPositionGenerator->generate().toVector4D();
	}
	return particles;
}