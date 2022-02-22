#include "QEngine.h"
#include "Render\Scene\Component\CustomShape\QCube.h"
#include "QDateTime"

QEngine::QEngine(int argc, char** argv)
	: QGuiApplication(argc, argv)
	, mWindow(std::make_shared<QRhiWindow>(QRhi::Implementation::Vulkan))
{
}

void QEngine::execRealtime()
{
	mWindow->showExposed();
	auto cube = std::make_shared<QCube>();
	scene()->addPrimitive(cube);
	while (true) {
		QGuiApplication::processEvents();
		cube->setPosition(QVector3D(0, 0, 0));
		cube->setScale(QVector3D(0.5, 0.5, 0.5));
		cube->setRotation(QVector3D(1, 1, 1) * QTime::currentTime().msecsSinceStartOfDay() / 10.0f);
		mWindow->requestUpdate();
	}
}

std::shared_ptr<QScene> QEngine::scene()
{
	return mWindow->scene();
}