#ifndef QEntity_h__
#define QEntity_h__

#include <QObject>
#include "Component\QTransformComponent.h"

class QWorld;

class QEntity :public QObject {
	Q_OBJECT
public:
	QEntity(QWorld* world) :mWorld(world) {}

	QWorld* world() {
		return mWorld;
	}
private:
	std::weak_ptr<QTransformComponent> mTransformComponent;
	QWorld* mWorld;
};

#endif // QEntity_h__