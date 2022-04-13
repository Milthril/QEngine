#ifndef QEngine_h__
#define QEngine_h__

#include <QApplication>
#include <QElapsedTimer>
#include <QDir>
#include "ECS\QWorld.h"
#include "private\qrhi_p.h"

#if defined(Engine)
#undef Engine
#endif
#define Engine (static_cast<QEngine *>(QEngine::instance()))

class QEngine :public QApplication
{
	Q_OBJECT
public:
	QEngine(int argc, char** argv, bool enableDebug = false);
	const std::shared_ptr<QWorld>& world();

	const QDir& assetDir() const { return mAssetDir;}
	void execGame();
protected:
	void registerMetaType();
	virtual void customInit();
	virtual void customUpdate();
private:
	std::shared_ptr<QWorld> mWorld;
	QDir mAssetDir;
	int64_t mLastTime;
	QElapsedTimer mTimer;
};

#endif // QEngine_h__
