#include "QParticleSystem.h"
#include "ParticleEmitter\QCubeEmitter.h"

QParticleSystem::QParticleSystem()
{
	mEmitter = std::make_shared<QCubeEmitter>();
}