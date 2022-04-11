#include "QRenderSystem.h"
#include "ECS\Component\IRenderableComponent.h"

void QRenderSystem::addRenderItem(IRenderableComponent* comp) {
	mRenderItemList << comp;
}

void QRenderSystem::removeRenderItem(IRenderableComponent* comp) {
	mRenderItemList.removeOne(comp);
}

