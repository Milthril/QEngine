#include "QShapeComponent.h"

void QShapeComponent::setVertices(const QVector<Vertex>& newVertices)
{
	mVertices = newVertices;
	bNeedUpdateVertex = true;
}

void QShapeComponent::setIndices(const QVector<uint32_t>& newIndices)
{
	mIndices = newIndices;
	bNeedUpdateIndex = true;
}

QRhiBuffer::Type QShapeComponent::getBufferType() const
{
	return mBufferType;
}

void QShapeComponent::setBufferType(QRhiBuffer::Type val)
{
	mBufferType = val;
}

std::shared_ptr<QMaterial> QShapeComponent::getMaterial() const
{
	return mMaterial;
}

void QShapeComponent::setMaterial(std::shared_ptr<QMaterial> val)
{
	mMaterial = val;
}