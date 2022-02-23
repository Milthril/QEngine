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

QColor QShapeComponent::getDefaultBaseColor() const
{
	return mDefaultBaseColor;
}

QVector4D QShapeComponent::getDefaultBaseColorVec4() const
{
	const QColor color = getDefaultBaseColor();
	return QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void QShapeComponent::setDefaultBaseColor(QColor val)
{
	if (mDefaultBaseColor == val)
		return;
	mDefaultBaseColor = val;
	for (auto& vertex : mVertices) {
		vertex.baseColor = getDefaultBaseColorVec4();
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