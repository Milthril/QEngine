#include <QGuiApplication>
#include "QDateTime"
#include "QEngine.h"
#include "qrandom.h"
#include "QDir"
#include "TimeDomainProvider.h"

const int CUBE_MAT_SIZE = 10;
const int CUBE_MAT_SPACING = 5;

class MyGame :public QEngine {
public:
	MyGame(int argc, char** argv)
		: QEngine(argc, argv) {
	}
	void init() override {
	}
	void customUpdate() override
	{
	}
};

int main(int argc, char** argv) {
	MyGame game(argc, argv);
	game.execGame();
}