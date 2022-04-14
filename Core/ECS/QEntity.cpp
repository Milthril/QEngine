#include "QEntity.h"
#include "ECS/QWorld.h"

QEntity::QEntity(QWorld* world) :
	mWorld(world) {
	mTransformComponent->setParent(this);
	mID = mIDStack++;
}

QMatrix4x4 QEntity::calculateMatrixMVP() {

	return mWorld->getMatrixVP()* mTransformComponent->calculateMatrix();
}
