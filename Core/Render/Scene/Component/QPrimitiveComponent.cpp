#include "QPrimitiveComponent.h"


QPrimitiveComponent::Topology QPrimitiveComponent::getTopology() const
{
	return mTopology;
}

void QPrimitiveComponent::setTopology(QPrimitiveComponent::Topology newTopology)
{
	if (mTopology == newTopology)
		return;
	mTopology = newTopology;
}