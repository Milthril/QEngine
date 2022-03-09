#include "QPrimitiveComponent.h"

const QVector<QPrimitiveComponent::Vertex>& QPrimitiveComponent::getVertices() const
{
	return mVertices;
}

void QPrimitiveComponent::setVertices(const QVector<Vertex>& newVertices)
{
	mVertices = newVertices;
}

const QVector<QPrimitiveComponent::Index>& QPrimitiveComponent::getIndices() const
{
	return mIndices;
}

void QPrimitiveComponent::setIndices(const QVector<Index>& newIndices)
{
	mIndices = newIndices;
}

void QPrimitiveComponent::autoFillIndices()
{
	QVector<Index> indices(mVertices.size());
	for (int i = 0; i < mVertices.size(); i++) {
		indices[i] = i;
	}
	setIndices(indices);
}

uint32_t QPrimitiveComponent::getVertexCount()
{
	return mVertices.size();
}

uint32_t QPrimitiveComponent::getIndexedCount()
{
	return mIndices.size();
}

QPrimitiveComponent::Topology QPrimitiveComponent::getTopology() const
{
	return mTopology;
}

void QPrimitiveComponent::setTopology(QPrimitiveComponent::Topology newTopology) {
	if (mTopology == newTopology)
		return;
	mTopology = newTopology;
}