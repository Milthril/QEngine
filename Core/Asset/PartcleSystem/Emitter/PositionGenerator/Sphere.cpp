﻿#include "Sphere.h"

namespace QPositionGenerator {

Sphere::Sphere() {
}

void Sphere::generate(QVector<Asset::ParticleSystem::Particle>& particles) {
	float radius = mRadius;
	if (mSolid) {
		radius = radius * mRandom.bounded(1.0);
	}
	for (auto& particle : particles) {
		float a = mRandom.bounded(M_PI);
		float b = mRandom.bounded(M_PI * 2);
		particle.position = QVector3D(radius * qSin(a) * qCos(b), radius * qSin(a) * qSin(b), radius * qCos(a)).toVector4D();
	}
}

}

