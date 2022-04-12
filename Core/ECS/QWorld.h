#ifndef QWorld_h__
#define QWorld_h__

#include <QObject>
#include "QEntity.h"
#include "QHash"

class QRenderSystem;

class QWorld: public QObject {
	Q_OBJECT
public:
	QEntity* createEntity(const QString& name);
	QEntity* getEntityById(const QEntity::ID id);
	QEntity* getEntityByName(const QString& name);
	bool removeEntity(const QString& name);
	QRenderSystem* renderSystem();
private:
	std::shared_ptr<QRenderSystem> mRenderSystem;
	QHash<QEntity::ID, QEntity*> mEntityHash;
};


#endif // QWorld_h__
