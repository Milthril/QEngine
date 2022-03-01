#include "QDateTime"
#include "QEngine.h"
#include "Render\Scene\Component\CustomShape\QCube.h"
#include "Render\Scene\Component\CustomShape\QSphere.h"
#include <QHBoxLayout>
#include <Windows.h>
#include <winuser.h>
#include "Render\Scene\Component\CustomShape\QText2D.h"

QEngine::QEngine(int argc, char** argv)
	: QApplication(argc, argv)
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::D3D11))
{
}

void QEngine::execRealtime()
{
	mWindow->show();
	mWindow->waitExposed();

	auto cube0 = std::make_shared<QCube>();
	auto sphere = std::make_shared<QSphere>();

	cube0->setPosition(QVector3D(-0.5, 0, 0));
	sphere->setPosition(QVector3D(0.5, 0, 0));
	cube0->setScale(QVector3D(0.4, 0.4, 0.4));
	sphere->setScale(QVector3D(0.4, 0.4, 0.4));

	//auto text = std::make_shared<QText2D>("ij");

	scene()->addPrimitive(sphere);
	scene()->addPrimitive(cube0);
	//scene()->addPrimitive(text);

	while (true) {
		QGuiApplication::processEvents();
		cube0->setDefaultBaseColor(QColor(0,100,200));
		cube0->setRotation(QVector3D(1, 1, 1) * QTime::currentTime().msecsSinceStartOfDay() / 10.0f);
		sphere->setRotation(QVector3D(1, 1, 1) * QTime::currentTime().msecsSinceStartOfDay() / 10.0f);
		//text->setText(QTime::currentTime().toString());
		//text->setText(QString::fromLocal8Bit("abcdefghijklmnopqrstuvwxyz"));
		//text->setText(QString::fromLocal8Bit("ij"));
		mWindow->requestUpdate();
	}
}

std::shared_ptr<QScene> QEngine::scene()
{
	return mWindow->scene();
}