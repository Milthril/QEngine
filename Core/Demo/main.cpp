#include <QGuiApplication>
#include "Window\QRhiWindow.h"

int main(int argc, char** argv) {
	QGuiApplication app(argc, argv);
	QRhiWindow window(QRhi::Implementation::Vulkan);
	window.show();
	return app.exec();
}