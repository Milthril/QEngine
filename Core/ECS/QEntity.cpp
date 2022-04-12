#include "QEntity.h"
#include "ECS/QWorld.h"

QMatrix4x4 QEntity::calculateMatrixMVP() {

	return mWorld->getMatrixVP()* mTransformComponent->calculateMatrix();
}
