#include "QParticleUpdater.h"

QParticleUpdater::QParticleUpdater()
{
	mUpdateCode =
		R"(outParticle.position = inParticle.position + inParticle.velocity;
outParticle.velocity = inParticle.velocity;
outParticle.scaling  = inParticle.scaling;
outParticle.rotation = inParticle.rotation;)";
}

void QParticleUpdater::setUpdateCode(QByteArray val)
{
	mUpdateCode = val;
	bNeedRecreate.active();
}