#include "ILightComponent.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"

void ILightComponent::setupEntity(QEntity* entity) {
	if (entity) {
		TheRenderSystem->renderer()->addLightItem(this);
	}
}

ILightComponent::~ILightComponent() {
	TheRenderSystem->renderer()->removeLightItem(this);
}

