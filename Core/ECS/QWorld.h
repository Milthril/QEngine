#ifndef QWorld_h__
#define QWorld_h__

#include <QObject>

class QRenderSystem;

class QWorld: public QObject {
	Q_OBJECT
public:
	QRenderSystem* renderSystem();
private:
	std::shared_ptr<QRenderSystem> mRenderSystem;
};


#endif // QWorld_h__
