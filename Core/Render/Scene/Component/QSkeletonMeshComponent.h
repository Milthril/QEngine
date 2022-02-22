#ifndef QSkeletonMeshComponent_h__
#define QSkeletonMeshComponent_h__

#include "QPrimitiveComponent.h"

class  QSkeletonMeshComponent :public QPrimitiveComponent {
public:
	QSceneComponent::Type type() override {
		return QSceneComponent::SkeletonMesh;
	}
};

#endif // QSkeletonMeshComponent_h__
