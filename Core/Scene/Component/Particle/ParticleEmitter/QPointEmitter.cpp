#include "QPointEmitter.h"

QVector<IParticleEmitter::Particle> QPointEmitter::update()
{
	QVector<IParticleEmitter::Particle> poo;
	poo.resize(1);
	for (auto& it : poo) {
		it.position = QVector4D(0, 0, 0, 0);
		it.velocity = QVector3D(0, 0.01, 0);
		it.scaling = QVector4D(1, 1, 1, 0);
	}
	return poo;
}