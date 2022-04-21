#include "QRenderSystem.h"
#include "IRenderable.h"
#include "Renderer/QRenderer.h"

void IRenderable::active() {
	TheRenderSystem->renderer()->addRenderItem(this);
}

void IRenderable::deactive() {
	TheRenderSystem->renderer()->removeRenderItem(this);
}
