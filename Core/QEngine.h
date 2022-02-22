#ifndef QEngine_h__
#define QEngine_h__

#include <QGuiApplication>
#include "Render\Scene\QScene.h"
#include "Window\QRhiWindow.h"

class QEngine :public QGuiApplication
{
public:
	QEngine(int argc, char** argv);
	void execRealtime();
	std::shared_ptr<QScene> scene();
private:
	std::shared_ptr<QRhiWindow> mWindow;
};

#endif // QEngine_h__
