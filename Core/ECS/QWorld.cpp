#include "QWorld.h"
#include "System\RenderSystem\QRenderSystem.h"
#include "Component\QCameraComponent.h"

QWorld::QWorld()
	: mCameraEntity(std::make_shared<QEntity>(this)){
	mCamera = mCameraEntity->addComponent<QCameraComponent>();
	mCamera->setupWindow(TheRenderSystem->window());
}

QWorld::~QWorld() {
}

QMatrix4x4 QWorld::getMatrixVP() {
	return mCamera->getMatrixVPWithCorr();
}

QEntity* QWorld::createEntity(const QString& name) {
	QEntity* entity = new QEntity(this);
	entity->setParent(this);
	entity->setObjectName(getVaildEntityName(name));
	mEntityHash[entity->GetId()] = entity;
	connect(entity, &QEntity::destroyed, this, [this](QObject* obj) {
		QEntity* entity = qobject_cast<QEntity*>(obj);
		if (entity) {
			mEntityHash.remove(entity->GetId());
		}
	});
	mCurrentEntity = entity;
	Q_EMIT worldChanged();
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
		removeEntity(entity);
	}
	return false;
}

bool QWorld::removeEntity(QEntity* entity) {
	if (entity) {
		entity->setParent(nullptr);
		mEntityHash.remove(entity->GetId());
		entity->deleteLater();
		if (mCurrentEntity == entity)
			setCurrentEntity(nullptr);
		Q_EMIT worldChanged();
		return true;
	}
	return false;
}

QString QWorld::getVaildEntityName(const QString& name) {
	QString newName = name;
	int index = 1;
	while (true) {
		bool flag = true;
		for (auto& entity : mEntityHash){
			if (entity->objectName() == newName)
				flag = false;
		}
		if (flag)
			return newName;
		newName = name + QString::number(index);
		index++;
	}
}

