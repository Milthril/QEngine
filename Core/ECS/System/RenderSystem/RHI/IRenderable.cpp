#include "IRenderable.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"

void IRenderable::active() {
	TheRenderSystem->renderer()->addRenderItem(this);
}

void IRenderable::deactive() {
	TheRenderSystem->renderer()->removeRenderItem(this);
}
