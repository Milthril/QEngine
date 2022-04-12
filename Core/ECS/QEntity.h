#ifndef QEntity_h__
#define QEntity_h__

#include <QObject>
#include "Component\QTransformComponent.h"

class QWorld;

class QEntity :public QObject {
	Q_OBJECT
public:
	QEntity(QWorld* world) :mWorld(world) {}

	QWorld* World() {
		return mWorld;
	}

	using ID = uint32_t;

	QEntity::ID GetId() {
		return mID;
	}

	QTransformComponent* getTransformComponent() {
		return mTransformComponent.get();
	}
public:
	std::shared_ptr<QTransformComponent> mTransformComponent;
private:
	QWorld* mWorld;
	QEntity::ID mID;
};

#endif // QEntity_h__