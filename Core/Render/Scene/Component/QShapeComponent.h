#ifndef QShapeComponent_h__
#define QShapeComponent_h__

#include "QPrimitiveComponent.h"

class QShapeComponent : public QPrimitiveComponent {
	Q_OBJECT
		Q_PROPERTY(QVector<Vertex> vertices READ vertices WRITE setVertices)
		Q_PROPERTY(QVector<uint32_t> indices READ indices WRITE setIndices)
		Q_PROPERTY(QVector<QImage> images READ images WRITE setImages)
		Q_PROPERTY(Topology topology READ topology WRITE setTopology)
public:
	const QVector<Vertex>& vertices() const;
	void setVertices(const QVector<Vertex>& newVertices);
	const QVector<uint32_t>& indices() const;
	void setIndices(const QVector<uint32_t>& newIndices);
	const QVector<QImage>& images() const;
	void setImages(const QVector<QImage>& newImages);
	Topology topology() const;
	void setTopology(Topology newTopology);

	QRhiBuffer::Type bufferType() const;
	void setBufferType(QRhiBuffer::Type val);
	QSceneComponent::Type type() override {
		return QSceneComponent::Type::Shape;
	}

private:
	QVector<Vertex> mVertices;
	QVector<uint32_t> mIndices;
	QVector<QImage> mImages;
	QRhiBuffer::Type mBufferType = QRhiBuffer::Type::Immutable;
	Topology mTopology;
};

#endif // QShapeComponent_h__
