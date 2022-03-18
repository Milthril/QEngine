#include "QParticleUpdater.h"

QParticleUpdater::QParticleUpdater()
{
	mUpdateCode = 
 R"(particle.life   = particle.life + 0.01;
particle.position = particle.position + particle.velocity;
particle.velocity = particle.velocity;
particle.scaling  = particle.scaling;
particle.rotation = particle.rotation;
    )";
}

void QParticleUpdater::setUpdateCode(QByteArray val)
{
	mUpdateCode = val;
	bNeedRecreate.active();
}