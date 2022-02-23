#ifndef QPrimitiveComponent_h__
#define QPrimitiveComponent_h__

#include "Render/Scene/QSceneComponent.h"

class QPrimitiveComponent :public QSceneComponent {
	Q_OBJECT
public:
	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;
		QVector4D baseColor;
	};
	using Index = uint32_t;
	using Topology = QRhiGraphicsPipeline::Topology;
};

#endif // QPrimitiveComponent_h__
