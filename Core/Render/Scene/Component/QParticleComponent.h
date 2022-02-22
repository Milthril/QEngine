#ifndef QParticleComponent_h__
#define QParticleComponent_h__

#include "QPrimitiveComponent.h"

class  QParticleComponent :public QPrimitiveComponent {
public:
	QSceneComponent::Type type() override {
		return QSceneComponent::Particle;
	}
};
#endif // QParticleComponent_h__
