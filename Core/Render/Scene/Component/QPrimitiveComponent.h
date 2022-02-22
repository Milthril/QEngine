#ifndef QPrimitiveComponent_h__
#define QPrimitiveComponent_h__

#include "Render/Scene/QSceneComponent.h"

class QPrimitiveComponent :public QSceneComponent {
	Q_OBJECT
		Q_PROPERTY(Topology topology READ topology WRITE setTopology)
public:
	struct Vertex {
		QVector3D getPosition;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;
		QVector4D baseColor;
	};
	using Index = uint32_t;
	enum Topology {		//图元拓扑结构
		Triangles,
		TriangleStrip,
		TriangleFan,
		Lines,
		LineStrip,
		Points
	};

	Topology topology() const;
	void setTopology(Topology newTopology);

private:
	Topology mTopology;
};

#endif // QPrimitiveComponent_h__
