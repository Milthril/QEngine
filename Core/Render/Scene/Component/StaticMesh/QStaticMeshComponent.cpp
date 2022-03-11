#include "QStaticMeshComponent.h"

void QStaticMeshComponent::setVertices(const QVector<Vertex>& newVertices)
{
	mVertices = newVertices;
	bNeedUpdateVertex.active();
}

void QStaticMeshComponent::setIndices(const QVector<uint32_t>& newIndices)
{
	mIndices = newIndices;
	bNeedUpdateIndex.active();
}

QRhiBuffer::Type QStaticMeshComponent::getBufferType() const
{
	return mBufferType;
}

void QStaticMeshComponent::setBufferType(QRhiBuffer::Type val)
{
	mBufferType = val;
}

std::shared_ptr<QMaterial> QStaticMeshComponent::getMaterial() const
{
	return mMaterial;
}

void QStaticMeshComponent::setMaterial(std::shared_ptr<QMaterial> val)
{
	mMaterial = val;
}

const QVector<QStaticMeshComponent::Vertex>& QStaticMeshComponent::getVertices() const
{
	return mVertices;
}

const QVector<QStaticMeshComponent::Index>& QStaticMeshComponent::getIndices() const
{
	return mIndices;
}

void QStaticMeshComponent::autoFillIndices()
{
	QVector<Index> indices(mVertices.size());
	for (int i = 0; i < mVertices.size(); i++) {
		indices[i] = i;
	}
	setIndices(indices);
}

uint32_t QStaticMeshComponent::getVertexCount()
{
	return mVertices.size();
}

uint32_t QStaticMeshComponent::getIndexedCount()
{
	return mIndices.size();
}

QStaticMeshComponent::Topology QStaticMeshComponent::getTopology() const
{
	return mTopology;
}

void QStaticMeshComponent::setTopology(QStaticMeshComponent::Topology newTopology) {
	if (mTopology == newTopology)
		return;
	mTopology = newTopology;
}