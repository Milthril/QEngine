#ifndef IParticleEmitter_h__
#define IParticleEmitter_h__

#include "..\QParticleSystem.h"

class IParticleEmitter :public QObject {
public:
	IParticleEmitter() {}

	using Particle = QParticleSystem::Particle;
	virtual QVector<Particle> update() = 0;
};

#endif // IParticleEmitter_h__
