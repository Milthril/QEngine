#include "IRenderableComponent.h"
#include "ECS/QEntity.h"
#include "ECS/QWorld.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"

IRenderableComponent::~IRenderableComponent() {
	if (mEntity) {
		mEntity->world()->renderSystem()->removeRenderItem(this);
	}
}

void IRenderableComponent::setupEntity(QEntity* entity) {
	IComponent::setupEntity(entity);
	entity->world()->renderSystem()->addRenderItem(this);
}

