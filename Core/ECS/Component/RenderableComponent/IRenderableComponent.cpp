#include "IRenderableComponent.h"
#include "ECS/QEntity.h"
#include "ECS/QWorld.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"

IRenderableComponent::~IRenderableComponent() {
	deactive();
}

void IRenderableComponent::setupEntity(QEntity* entity) {
	if (entity) {
		IComponent::setupEntity(entity);
		active();
	}
}

