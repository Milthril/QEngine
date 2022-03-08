#ifndef QPrimitiveComponent_h__
#define QPrimitiveComponent_h__

#include "Render/Scene/QSceneComponent.h"
#include "Render/Material/QMaterial.h"

class QPrimitiveComponent :public QSceneComponent {
	Q_OBJECT
		Q_PROPERTY(Topology topology READ getTopology WRITE setTopology)
		Q_PROPERTY(QVector<Vertex> vertices READ getVertices WRITE setVertices)
		Q_PROPERTY(QVector<uint32_t> indices READ getIndices WRITE setIndices)
public:
	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;
	};
	using Index = uint32_t;
	using Topology = QRhiGraphicsPipeline::Topology;

	Topology getTopology() const;
	virtual void setTopology(Topology newTopology);

	const QVector<Vertex>& getVertices() const;
	virtual void setVertices(const QVector<Vertex>& newVertices);

	const QVector<Index>& getIndices() const;
	virtual void setIndices(const QVector<Index>& newIndices);

	void autoFillIndices();
	uint32_t getIndexedCount();

protected:
	Topology mTopology = Topology::Triangles;
	QVector<Vertex> mVertices;
	QVector<Index> mIndices;
};

#endif // QPrimitiveComponent_h__
