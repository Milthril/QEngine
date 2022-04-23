#include "ParticleSystem.h"
#include "Emitter\QParticleEmitter.h"
#include "QParticleUpdater.h"
#include "Serialization\QSerialization.h"

namespace Asset {
Asset::ParticleSystem::ParticleSystem() {
	mEmitter = std::make_shared<QParticleEmitter>();
	mUpdater = std::make_shared<QParticleUpdater>();
}

void ParticleSystem::serialize(QDataStream& out) {
	out << serializeQObject(mEmitter.get());
	out << mUpdater->getUpdateCode();
	mUpdater->serialize(out);
}

void ParticleSystem::deserialize(QDataStream& in) {
	QByteArray emitter;
	in >> emitter;
	deserializeQObject(mEmitter.get(), emitter);
	QByteArray updateCode;
	in >> updateCode;
	mUpdater->deserialize( in);
	mUpdater->setUpdateCode(updateCode);
}

}

