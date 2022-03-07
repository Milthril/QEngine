#include <QGuiApplication>
#include "MyGame.h"

int main(int argc, char** argv) {
	MyGame game(argc, argv);
	game.execGame();
}