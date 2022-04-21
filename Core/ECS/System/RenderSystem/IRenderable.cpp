#include "QRenderSystem.h"
#include "IRenderable.h"

void IRenderable::active() {
	TheRenderSystem->addRenderItem(this);
}

void IRenderable::deactive() {
	TheRenderSystem->removeRenderItem(this);
}
