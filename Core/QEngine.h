#ifndef QEngine_h__
#define QEngine_h__

#include <QApplication>
#include "Scene\QScene.h"
#include "RHI\QRhiWindow.h"

#if defined(Engine)
#undef Engine
#endif
#define Engine (static_cast<QEngine *>(QEngine::instance()))
#define RHI (static_cast<QEngine *>(QEngine::instance()))->getRHI()

class QEngine :public QApplication
{
public:
	QEngine(int argc, char** argv);
	std::shared_ptr<QScene> scene();
	std::shared_ptr<QRhiWindow> window();
	std::shared_ptr<QSceneRenderer> renderer();
	void execGame();
	QRhi* getRHI();
protected:
	virtual void init();
	virtual void update();
private:
	std::shared_ptr<QRhiWindow> mWindow;
	std::shared_ptr<QScene> mScene;
	std::shared_ptr<QSceneRenderer>  mRenderer;
};

#endif // QEngine_h__
