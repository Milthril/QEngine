#ifndef QPrimitiveComponent_h__
#define QPrimitiveComponent_h__

#include "Render/Scene/QSceneComponent.h"

const uint32_t QMATERIAL_DIFFUSE = 0;

typedef QHash<uint32_t, QImage> QMaterial;

class QPrimitiveComponent :public QVisibleComponent {
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

	enum Topology {		//图元拓扑结构
		Triangles,
		TriangleStrip,
		TriangleFan,
		Lines,
		LineStrip,
		Points
	};
	enum Type {
		Static,			//顶点数据不会更新
		Dynamic			//顶点数据将没帧都更新
	};

private:
	Topology mTopology;
	QVector<Vertex> mVertices;
	QVector<uint32_t> mIndices;
};

#endif // QPrimitiveComponent_h__
