#ifndef QShapeComponent_h__
#define QShapeComponent_h__

#include "QPrimitiveComponent.h"
#include "Render\Material\QMaterialBaseColor.h"

class QShapeComponent : public QPrimitiveComponent {
	Q_OBJECT
public:
	virtual void setVertices(const QVector<Vertex>& newVertices) override;

	virtual void setIndices(const QVector<uint32_t>& newIndices) override;

	QRhiBuffer::Type getBufferType() const;
	void setBufferType(QRhiBuffer::Type val);

	std::shared_ptr<QMaterial> getMaterial() const;
	void setMaterial(std::shared_ptr<QMaterial> val);

	QSceneComponent::Type type() override {
		return QSceneComponent::Type::Shape;
	}
	uint8_t bNeedUpdateVertex : 1 = 0;
	uint8_t bNeedUpdateIndex : 1 = 0;
	uint8_t bNeedUpdateTexture : 1 = 0;
private:
	QRhiBuffer::Type mBufferType = QRhiBuffer::Type::Immutable;
	std::shared_ptr<QMaterial> mMaterial = std::make_shared<QMaterialBaseColor>();
};

#endif // QShapeComponent_h__