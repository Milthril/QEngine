#ifndef QStaticMeshComponent_h__
#define QStaticMeshComponent_h__

#include "Render/Scene/QSceneComponent.h"

class QStaticMeshNode {
	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoords;
	};
	QVector<Vertex> mVertices;
	QVector<uint32_t> mIndices;
	QMatrix4x4 mLocalMatrix;
};

class QStaticMeshComponent :public QVisibleComponent {
	struct QMaterial {
		QImage diffuse;
	};
};

#endif // QStaticMeshComponent _h__
