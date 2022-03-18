#ifndef QCube_h__
#define QCube_h__

#include "Scene/Component/StaticMesh/QStaticMeshComponent.h"

class QCube :public QStaticMeshComponent {
	Q_OBJECT
	REGISTER_SUBCLASS(QStaticMeshComponent, QCube)
public:
	Q_INVOKABLE QCube();
};

#endif // QCube_h__