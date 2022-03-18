#include "QParticleSystem.h"
#include "QParticleEmitter.h"

QParticleSystem::QParticleSystem()
{
	mEmitter = std::make_shared<QParticleEmitter>();
	mUpdater = std::make_shared<QParticleUpdater>();
}