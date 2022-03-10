#ifndef QStaticMeshComponent_h__
#define QStaticMeshComponent_h__

#include "Render\Scene\Component\QPrimitiveComponent.h"
#include "Render\Material\QMaterialBaseColor.h"

class QStaticMeshComponent : public QPrimitiveComponent {
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
	uint8_t bNeedUpdateVertex : 1 = 0;
	uint8_t bNeedUpdateIndex : 1 = 0;
	uint8_t bNeedUpdateTexture : 1 = 0;
private:
	QRhiBuffer::Type mBufferType = QRhiBuffer::Type::Immutable;
	std::shared_ptr<QMaterial> mMaterial = std::make_shared<QMaterialBaseColor>();
	Topology mTopology = Topology::Triangles;
	QVector<Vertex> mVertices;
	QVector<Index> mIndices;
};

#endif // QStaticMeshComponent_h__