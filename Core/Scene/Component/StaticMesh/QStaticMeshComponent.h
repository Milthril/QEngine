#ifndef QStaticMeshComponent_h__
#define QStaticMeshComponent_h__

#include "Scene\Component\QPrimitiveComponent.h"
#include "ExtType\QSubClass.h"

class QStaticMeshComponent : public QPrimitiveComponent {
	Q_OBJECT
		Q_PROPERTY(Topology Topology READ getTopology WRITE setTopology SCRIPTABLE false)
		Q_PROPERTY(QVector<Vertex> Vertices READ getVertices WRITE setVertices SCRIPTABLE false)
		Q_PROPERTY(QVector<uint32_t> Indices READ getIndices WRITE setIndices SCRIPTABLE false)
		Q_PROPERTY(std::shared_ptr<QMaterial> Material READ getMaterial WRITE setMaterial)
public:
	QStaticMeshComponent();
	virtual ~QStaticMeshComponent();
	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;
	};
	Topology getTopology() const;
	virtual void setTopology(Topology newTopology);

	const QVector<Vertex>& getVertices() const;
	void setVertices(const QVector<Vertex>& newVertices);

	const QVector<Index>& getIndices() const;
	void setIndices(const QVector<Index>& newIndices);

	QRhiBuffer::Type getBufferType() const;
	void setBufferType(QRhiBuffer::Type val);

	std::shared_ptr<QMaterial> getMaterial() const;
	void setMaterial(std::shared_ptr<QMaterial> val);

	void autoFillIndices();

	uint32_t getVertexCount();
	uint32_t getIndexedCount();

	QSceneComponent::ProxyType type() override {
		return QSceneComponent::ProxyType::StaticMesh;
	}
	QRhiSignal bNeedUpdateVertex;
	QRhiSignal bNeedUpdateIndex;
private:
	QRhiBuffer::Type mBufferType = QRhiBuffer::Type::Immutable;
	std::shared_ptr<QMaterial> mMaterial;
	Topology mTopology = Topology::Triangles;
	QVector<Vertex> mVertices;
	QVector<Index> mIndices;
};

Q_DECLARE_METATYPE(QSubClass<QStaticMeshComponent>) 

#endif // QStaticMeshComponent_h__