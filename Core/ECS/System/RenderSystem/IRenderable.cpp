#include "QRenderSystem.h"
#include "IRenderable.h"

void IRenderable::active() {
	QRenderSystem::instance()->addRenderItem(this);
}

void IRenderable::deactive() {
	QRenderSystem::instance()->removeRenderItem(this);
}
