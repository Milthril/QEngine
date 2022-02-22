#ifndef QStaticMeshComponent_h__
#define QStaticMeshComponent_h__

#include "Render/Scene/QSceneComponent.h"
#include "QPrimitiveComponent.h"

class QStaticMeshNode {
	QVector<QPrimitiveComponent::Vertex> mVertices;
	QVector<uint32_t> mIndices;
	QMatrix4x4 mLocalMatrix;
};

class QStaticMeshComponent :public QPrimitiveComponent {
	struct QMaterial {
		QImage diffuse;
	};
public:
	QSceneComponent::Type type() override;

};

#endif // QStaticMeshComponent _h__
