#ifndef QLightComponent_h__
#define QLightComponent_h__

#include "Render/Scene/QSceneComponent.h"

class  QParticleComponent :public QSceneComponent {
public:
	QSceneComponent::Type type() override {
		return QSceneComponent::Light;
	}
};
#endif // QLightComponent_h__
