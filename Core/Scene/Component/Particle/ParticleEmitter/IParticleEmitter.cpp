#include "IParticleEmitter.h"

QVector<IParticleEmitter::Particle> IParticleEmitter::update()
{
	QVector<IParticleEmitter::Particle> particles(qMax(0, mNumOfTick));
	for (IParticleEmitter::Particle& particle : particles) {
		handParticle(particle);
		particle.scaling = mScaling.toVector4D();
	}
	return particles;
}