#ifndef QEngine_h__
#define QEngine_h__

#include <QApplication>
#include "Scene\QScene.h"
#include "RHI\QRhiWindow.h"

#if defined(Engine)
#undef Engine
#endif
#define Engine (static_cast<QEngine *>(QEngine::instance()))

class QEngine :public QApplication
{
public:
	QEngine(int argc, char** argv);
	std::shared_ptr<QScene> scene();
	std::shared_ptr<QRhiWindow> window();
	void execGame();
protected:
	virtual void onGameLoop();
private:
	std::shared_ptr<QRhiWindow> mWindow;
};

#endif // QEngine_h__
