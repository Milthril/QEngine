#ifndef QSkyBoxComponent_h__
#define QSkyBoxComponent_h__

#include "QPrimitiveComponent.h"

class QSkyBoxComponent :public QPrimitiveComponent {
public:
	QSceneComponent::Type type() override {
		return QSceneComponent::SkyBox;
	}	
};

#endif // QSkyBoxComponent_h__
