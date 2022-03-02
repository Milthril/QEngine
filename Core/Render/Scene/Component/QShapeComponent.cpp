#include "QShapeComponent.h"

const QVector<QPrimitiveComponent::Vertex>& QShapeComponent::getVertices() const
{
	return mVertices;
}

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

const QVector<uint32_t>& QShapeComponent::getIndices() const
{
	return mIndices;
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

QVector4D QShapeComponent::getDefaultBaseColor() const
{
	return mDefaultBaseColor;
}

void QShapeComponent::setDefaultBaseColor(QVector4D val)
{
	if (mDefaultBaseColor == val)
		return;
	mDefaultBaseColor = val;
	for (auto& vertex : mVertices) {
		vertex.baseColor = val;
	}
	if (mTexture.isNull()) {
		bNeedUpdateVertex = true;
	}
	else {
		mTexture = {};
		bNeedResetProxy = true;
	}
}

QImage QShapeComponent::getTexture() const
{
	return mTexture;
}

void QShapeComponent::setTexture(QImage val)
{
	if (!mTexture.isNull()) {
		bNeedResetProxy = true;
	}
	else {
		bNeedUpdateTexture = true;
	}
	mTexture = val;
}

QShapeComponent::Topology QShapeComponent::getTopology() const
{
	return mTopology;
}

void QShapeComponent::setTopology(QShapeComponent::Topology newTopology)
{
	if (mTopology == newTopology)
		return;
	mTopology = newTopology;
}