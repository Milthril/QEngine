#ifndef QEngine_h__
#define QEngine_h__

#include <QApplication>
#include "Render\Scene\QScene.h"
#include "Window\QRhiWindow.h"

class QEngine :public QApplication
{
public:
	QEngine(int argc, char** argv);
	std::shared_ptr<QScene> scene();
	void execGame();
protected:
	virtual void onGameLoop();

protected:
	std::shared_ptr<QRhiWindow> mWindow;
};

#endif // QEngine_h__
