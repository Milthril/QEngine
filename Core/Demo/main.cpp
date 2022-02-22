#include <QGuiApplication>
#include "Window\QRhiWindow.h"
#include "Render\Scene\QScene.h"
#include "Render\Scene\Component\CustomShape\QCube.h"

int main(int argc, char** argv) {
	QGuiApplication app(argc, argv);
	QRhiWindow window(QRhi::Implementation::Vulkan);
	window.show();
	window.waitExposed();
	window.scene()->addPrimitive(std::make_shared<QCube>());
	return app.exec();
}