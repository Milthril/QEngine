#include "QEntity.h"
#include "ECS/QWorld.h"

QEntity::QEntity(QWorld* world) :
	mWorld(world) {
	mTransformComponent->setParent(this);
	mID = mIDStack++;
}

void QEntity::addComponent(IComponent* component) {
	if (!component)
		return;
	component->setParent(this);
	component->setupEntity(this);
	mComponentHash[component->metaObject()->metaType().id()] = component;
}

void QEntity::removeComponent(IComponent* component) {
	if (!component)
		return;
	component->setParent(nullptr);
	component->setupEntity(nullptr);
	mComponentHash.remove(component->metaObject()->metaType().id());
	delete component;
}

QMatrix4x4 QEntity::calculateMatrixMVP() {

	return mWorld->getMatrixVP()* mTransformComponent->calculateMatrix();
}
