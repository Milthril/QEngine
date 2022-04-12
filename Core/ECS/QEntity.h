#ifndef QEntity_h__
#define QEntity_h__

#include <QObject>
#include <QHash>
#include "Component\QTransformComponent.h"

class QWorld;

class QEntity :public QObject {
	Q_OBJECT
public:
	QEntity(QWorld* world) :mWorld(world) {}

	QWorld* World() {
		this->metaObject()->metaType().id();
		return mWorld;
	}

	using ID = uint32_t;

	template<typename Component>
	Component* addComponent() {
		if (mComponentHash.contains(Component::staticMetaObject.metaType().id())) {
			return nullptr;
		}
		Component* component = new Component;
		component->setParent(this);
		component->setupEntity(this);
		mComponentHash[component->metaObject()->metaType().id()] = component;
		return component;
	}

	template<typename Component>
	Component* getComponent() {
		return mComponentHash.value(Component::staticMetaObject.metaType().id(), nullptr);
	}
	template<typename Component>
	bool removeComponent() {
		return mComponentHash.remove(Component::staticMetaObject.metaType().id());
	}
	QEntity::ID GetId() {
		return mID;
	}

	QTransformComponent* getTransformComponent() {
		return mTransformComponent.get();
	}

	QMatrix4x4 calculateMatrixMVP();
public:
	std::shared_ptr<QTransformComponent> mTransformComponent = std::make_shared<QTransformComponent>();
private:
	QWorld* mWorld;
	QEntity::ID mID;
	QHash<int, IComponent*> mComponentHash;
};

#endif // QEntity_h__