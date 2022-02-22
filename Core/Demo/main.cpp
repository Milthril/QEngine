#include <QGuiApplication>
#include "Window\QRhiWindow.h"
#include "QEngine.h"

int main(int argc, char** argv) {
	QEngine app(argc, argv);
	app.execRealtime();
}