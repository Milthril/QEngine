#include "QEngineEditorApplication.h"
#include <QMetaType>

int main(int argc, char** argv) {
	QEngineEditorApplication qEngine(argc, argv);
	qEngine.execGame();
}