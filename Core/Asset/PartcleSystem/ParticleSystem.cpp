#include "ParticleSystem.h"
#include "Emitter\QParticleEmitter.h"
#include "QParticleUpdater.h"
#include "Serialization\QSerialization.h"

namespace Asset {
Asset::ParticleSystem::ParticleSystem() {
	mEmitter = std::make_shared<QParticleEmitter>();
	mUpdater = std::make_shared<QParticleUpdater>();
}

QDataStream& operator<<(QDataStream& out, const ParticleSystem& var) {
	out<< serializeQObject(var.mEmitter.get());
	out << var.mUpdater->getUpdateCode();
	var.mUpdater->save(out);
	return out;
}

QDataStream& operator>>(QDataStream& in, ParticleSystem& var) {
	QByteArray emitter;
	in >> emitter;
	deserializeQObject(var.mEmitter.get(), emitter);
	QByteArray updateCode;
	in >> updateCode;
	var.mUpdater->read(in);
	var.mUpdater->setUpdateCode(updateCode);
	return in;
}

}

