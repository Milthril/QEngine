#ifndef QSkeletonMeshComponent_h__
#define QSkeletonMeshComponent_h__

#include "Render\Scene\Component\QPrimitiveComponent.h"

class  QSkeletonMeshComponent :public QPrimitiveComponent {
public:
	QSceneComponent::ProxyType type() override {
		return QSceneComponent::SkeletonMesh;
	}
};

#endif // QSkeletonMeshComponent_h__
