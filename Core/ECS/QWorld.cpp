#include "QWorld.h"

std::shared_ptr<QEntity> QWorld::createEntity() {
	return std::make_shared<QEntity>(this);
}

QRenderSystem* QWorld::renderSystem() {
	return mRenderSystem.get();
}

