#ifndef QText2DComponent_h__
#define QText2DComponent_h__

#include "QPrimitiveComponent.h"

class QText2DComponent :public QPrimitiveComponent {
public:
	QSceneComponent::Type type() override;

};

#endif // QText2DComponent_h__