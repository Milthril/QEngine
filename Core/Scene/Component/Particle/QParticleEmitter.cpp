#include "QParticleEmitter.h"
#include "PositionGenerator\QCubeGenerator.h"

QParticleEmitter::QParticleEmitter()
{
	mPositionGenerator.setSubClass<QCubeGenerator>();
}

QVector<QParticleEmitter::Particle> QParticleEmitter::update()
{
	QVector<QParticleEmitter::Particle> particles(qMax(0, mNumOfTick));
	for (QParticleEmitter::Particle& particle : particles) {
		particle.scaling = mScaling.toVector4D();
	}
	mPositionGenerator->generate(particles);
	return particles;
}