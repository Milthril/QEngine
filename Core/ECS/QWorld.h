#ifndef QWorld_h__
#define QWorld_h__

#include <QObject>
#include "QEntity.h"
#include "QHash"


class QRenderSystem;
class QCameraComponent;

class QWorld: public QObject {
	Q_OBJECT
public:
	QWorld();
	QMatrix4x4 getMatrixVP();
	QEntity* createEntity(const QString& name="Entity");
	QEntity* getEntityById(const QEntity::ID id);
	QEntity* getEntityByName(const QString& name);
	bool removeEntity(const QString& name);
	bool removeEntity(QEntity* entity);

private:
	QString getVaildEntityName(const QString& name);
Q_SIGNALS:
	void worldChanged();
private:
	QHash<QEntity::ID, QEntity*> mEntityHash;
	std::shared_ptr<QEntity> mCameraEntity;
	QCameraComponent* mCamera;
};


#endif // QWorld_h__
