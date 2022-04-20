#ifndef QEngineCoreApplication_h__
#define QEngineCoreApplication_h__

#include <QApplication>
#include <QElapsedTimer>
#include <QDir>
#include "ECS\QWorld.h"
#include "private\qrhi_p.h"

#if defined(TheEngine)
#undef Engine
#endif
#define TheEngine (static_cast<QEngineCoreApplication *>(QEngineCoreApplication::instance()))

class QEngineCoreApplication :public QApplication
{
	Q_OBJECT
public:
	QEngineCoreApplication(int argc, char** argv, bool enableDebug = false);
	const std::shared_ptr<QWorld>& world();
	const QDir& assetDir() const { return mAssetDir;}
	void execGame();

	void processEvents();
protected:
	virtual void customInit();
	virtual void customUpdate();
	virtual void customRelease();
private:
	std::shared_ptr<QWorld> mWorld;
	QDir mAssetDir;
	int64_t mLastTime;
	QElapsedTimer mTimer;
};

#endif // QEngineCoreApplication_h__
