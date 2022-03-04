#ifndef QShapeComponent_h__
#define QShapeComponent_h__

#include "QPrimitiveComponent.h"

class QShapeComponent : public QPrimitiveComponent {
	Q_OBJECT
		Q_PROPERTY(QVector<Vertex> vertices READ getVertices WRITE setVertices)
		Q_PROPERTY(QVector<uint32_t> indices READ getIndices WRITE setIndices)
		Q_PROPERTY(QVector4D defaultBaseColor READ getDefaultBaseColor WRITE setDefaultBaseColor)
		Q_PROPERTY(QImage texture READ getTexture WRITE setTexture)
public:
	const QVector<Vertex>& getVertices() const;
	void setVertices(const QVector<Vertex>& newVertices);

	const QVector<uint32_t>& getIndices() const;
	void setIndices(const QVector<uint32_t>& newIndices);

	QRhiBuffer::Type getBufferType() const;
	void setBufferType(QRhiBuffer::Type val);

	QVector4D getDefaultBaseColor() const;
	void setDefaultBaseColor(QVector4D val);

	QImage getTexture() const;
	void setTexture(QImage val);

	QSceneComponent::Type type() override {
		return QSceneComponent::Type::Shape;
	}

	uint8_t bNeedUpdateVertex : 1 = 0;
	uint8_t bNeedUpdateIndex : 1 = 0;
	uint8_t bNeedUpdateTexture : 1 = 0;
private:
	QVector<Vertex> mVertices;
	QVector<uint32_t> mIndices;
	QImage mTexture;
	QVector4D mDefaultBaseColor = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	QRhiBuffer::Type mBufferType = QRhiBuffer::Type::Immutable;
};

#endif // QShapeComponent_h__
// git remote add origin http://git.xinyuanact.com/techR/axon/QEngine.git