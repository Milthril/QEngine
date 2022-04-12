#include "QRenderSystem.h"
#include "ECS\Component\IRenderableComponent.h"


void IRenderable::active() {
	QRenderSystem::instance()->addRenderItem(this);
}

void IRenderable::deactive() {
	QRenderSystem::instance()->removeRenderItem(this);
}
