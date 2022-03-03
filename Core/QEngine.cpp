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
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::OpenGLES2))
{
}

void QEngine::execRealtime()
{
	mWindow->show();
	mWindow->waitExposed();

	auto cube = std::make_shared<QCube>();
	cube->setPosition(QVector3D(-0.8, 0.8, 0));
	cube->setScale(QVector3D(0.4, 0.4, 0.4));

	auto sphere = std::make_shared<QSphere>();
	sphere->setPosition(QVector3D(0.8, 0.8, 0));
	sphere->setScale(QVector3D(0.4, 0.4, 0.4));

	auto text = std::make_shared<QText2D>("default");
	text->setPosition(QVector3D(0, -0.8, 0));

	scene()->addPrimitive(sphere);
	scene()->addPrimitive(cube);
	scene()->addPrimitive(text);

	while (true) {
		QGuiApplication::processEvents();

		float time = QTime::currentTime().msecsSinceStartOfDay();
		cube->setDefaultBaseColor(QVector4D(0.1, 0.5, 0.9, 1) * (20 + 20 * qSin(time / 400)));
		cube->setRotation(QVector3D(1, 1, 1) * time / 10.0f);

		sphere->setDefaultBaseColor(QVector4D(0.8, 0.5, 0.4, 1) * (20 + 20 * qSin(time / 500)));

		text->setText(QTime::currentTime().toString());
		text->setDefaultBaseColor(QVector4D(0.5, 0.1, 0.8, 1) * (20 + 20 * qSin(time / 600)));

		mWindow->requestUpdate();
	}
}

std::shared_ptr<QScene> QEngine::scene()
{
	return mWindow->scene();
}