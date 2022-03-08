#include "QShapeComponent.h"

void QShapeComponent::setVertices(const QVector<Vertex>& newVertices)
{
	mVertices = newVertices;
	if (mBufferType == QRhiBuffer::Dynamic) {
		bNeedUpdateVertex = true;
	}
	else {
		bNeedResetProxy = true;
	}
}

void QShapeComponent::setIndices(const QVector<uint32_t>& newIndices)
{
	mIndices = newIndices;
	if (mBufferType == QRhiBuffer::Dynamic) {
		bNeedUpdateIndex = true;
	}
	else {
		bNeedResetProxy = true;
	}
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
