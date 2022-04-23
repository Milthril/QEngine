#include "QParticleUpdater.h"

QParticleUpdater::QParticleUpdater()
{
	setDataVec3("Force", QVector3D(0, 0.0098, 0));
	mUpdateCode =
		R"(outParticle.position = inParticle.position + inParticle.velocity;
outParticle.velocity = inParticle.velocity + UBO.Force;
outParticle.scaling  = inParticle.scaling;
outParticle.rotation = inParticle.rotation;)";
}

void QParticleUpdater::setUpdateCode(QByteArray val)
{
	mUpdateCode = val;
	bNeedRecreate.active();
}