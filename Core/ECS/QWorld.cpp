#include "QWorld.h"
#include "System\RenderSystem\QRenderSystem.h"
#include "Component\QCameraComponent.h"

QWorld::QWorld()
	: mCameraEntity(std::make_shared<QEntity>(this)){
	mCamera = mCameraEntity->addComponent<QCameraComponent>();
	mCamera->setupWindow(QRenderSystem::instance()->window());
}

QMatrix4x4 QWorld::getMatrixVP() {
	return mCamera->getMatrixVP();
}

QEntity* QWorld::createEntity(const QString& name) {
	QEntity* entity = new QEntity(this);
	entity->setParent(this);
	entity->setObjectName(name);
	mEntityHash[entity->GetId()] = entity;
	connect(entity, &QEntity::destroyed, this, [this](QObject* obj) {
		QEntity* entity = qobject_cast<QEntity*>(obj);
		if (entity) {
			mEntityHash.remove(entity->GetId());
		}
	});
	return entity;
}

QEntity* QWorld::getEntityById(const QEntity::ID id) {
	return mEntityHash.value(id,nullptr);
}

QEntity* QWorld::getEntityByName(const QString& name) {
	for (auto& entity : mEntityHash) {
		if (entity->objectName() == name)
			return entity;
	}
	return nullptr;;
}

bool QWorld::removeEntity(const QString& name) {
	QEntity* entity = getEntityByName(name);
	if (entity) {
		entity->setParent(nullptr);
		entity->deleteLater();
		return true;
	}
	return false;
}

