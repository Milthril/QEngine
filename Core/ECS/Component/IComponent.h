#ifndef IComponent_h__
#define IComponent_h__

#include <QObject>

class QEntity;

class IComponent :public QObject {
	friend class QEntity;
	Q_OBJECT
public:
	IComponent() {}

	virtual void setupEntity(QEntity* entity) {
		mEntity = entity;
	}
protected:
	QEntity* mEntity = nullptr;
};

#endif // QComponent_h__