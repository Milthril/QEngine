#include "QEngineEditorApplication.h"


void QEngineMessageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& text) {
	TheEngine->getEditorWindow()->mConsolePanel.addMessage(type, ctx, text);
}
