#include "QParticleEmitter.h"

QParticleEmitter::QParticleEmitter()
{
}

QVector<QParticleEmitter::Particle> QParticleEmitter::update()
{
	QVector<QParticleEmitter::Particle> particles(qMax(0, mNumOfTick));
	for (QParticleEmitter::Particle& particle : particles) {
		particle.scaling = mScaling.toVector4D();
	}
	mPositionGenerator->generate(particles);
	mVelocityGenerator->generate(particles);
	return particles;
}