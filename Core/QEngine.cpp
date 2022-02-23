#include "QEngine.h"
#include "Render\Scene\Component\CustomShape\QCube.h"
#include "QDateTime"
#include "Render\Scene\Component\QText2DComponent.h"
#include "Render\Scene\Component\CustomShape\QSphere.h"

QEngine::QEngine(int argc, char** argv)
	: QGuiApplication(argc, argv)
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::Vulkan))
{
}

void QEngine::execRealtime()
{
	mWindow->showExposed();
	auto cube0 = std::make_shared<QCube>();
	auto sphere = std::make_shared<QSphere>();

	cube0->setPosition(QVector3D(-0.5, 0, 0));
	sphere->setPosition(QVector3D(0.5, 0, 0));
	cube0->setScale(QVector3D(0.4, 0.4, 0.4));
	sphere->setScale(QVector3D(0.4, 0.4, 0.4));

	auto text = std::make_shared<QText2DComponent>();
	text->setText("Text");

	scene()->addPrimitive(sphere);
	scene()->addPrimitive(cube0);
	scene()->addPrimitive(text);

	while (true) {
		QGuiApplication::processEvents();
		cube0->setDefaultBaseColor(Qt::black);
		cube0->setRotation(QVector3D(1, 1, 1) * QTime::currentTime().msecsSinceStartOfDay() / 10.0f);
		sphere->setRotation(QVector3D(1, 1, 1) * QTime::currentTime().msecsSinceStartOfDay() / 10.0f);
		mWindow->requestUpdate();
		text->setText(QTime::currentTime().toString());
	}
}

std::shared_ptr<QScene> QEngine::scene()
{
	return mWindow->scene();
}