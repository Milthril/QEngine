#include "ParticleEmitter\QPointEmitter.h"
#include "QParticleSystem.h"

QParticleSystem::QParticleSystem()
{
	mEmitter = std::make_shared<QPointEmitter>();
}