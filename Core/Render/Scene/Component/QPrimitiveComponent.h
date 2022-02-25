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

/**
 *	用于将QColor转QVector4D
 */
QVector4D operator<<(QVector4D& vec4, const QColor& color);

#endif // QPrimitiveComponent_h__
