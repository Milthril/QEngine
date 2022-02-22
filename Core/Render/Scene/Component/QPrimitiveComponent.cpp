#include "QPrimitiveComponent.h"

QPrimitiveComponent::Topology QPrimitiveComponent::topology() const
{
	return mTopology;
}

void QPrimitiveComponent::setTopology(QPrimitiveComponent::Topology newTopology)
{
	if (mTopology == newTopology)
		return;
	mTopology = newTopology;
}

