#ifndef QWorld_h__
#define QWorld_h__

#include <QObject>
#include "QEntity.h"

class QRenderSystem;

class QWorld: public QObject {
	Q_OBJECT
public:
	std::shared_ptr<QEntity> createEntity();
	QRenderSystem* renderSystem();
private:
	std::shared_ptr<QRenderSystem> mRenderSystem;
};


#endif // QWorld_h__
