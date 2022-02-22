#include "QShapeComponent.h"

const QVector<QPrimitiveComponent::Vertex>& QShapeComponent::vertices() const
{
	return mVertices;
}

void QShapeComponent::setVertices(const QVector<Vertex>& newVertices)
{
	mVertices = newVertices;
}

const QVector<uint32_t>& QShapeComponent::indices() const
{
	return mIndices;
}

void QShapeComponent::setIndices(const QVector<uint32_t>& newIndices)
{
	if (mIndices == newIndices)
		return;
	mIndices = newIndices;
}

const QVector<QImage>& QShapeComponent::images() const
{
	return mImages;
}

void QShapeComponent::setImages(const QVector<QImage>& newImages)
{
	if (mImages == newImages)
		return;
	mImages = newImages;
}