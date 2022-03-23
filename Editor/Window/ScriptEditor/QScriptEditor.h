#ifndef QScriptEditor_h__
#define QScriptEditor_h__

#include "KDDockWidgets\DockWidget.h"

class QLuaScript;
class LuaEditor;
class QPushButton;

class QScriptEditor :public KDDockWidgets::DockWidget {
public:
	static QScriptEditor* instance();
	void edit(std::shared_ptr<QLuaScript> script);
private:
	QScriptEditor();
private:
	std::shared_ptr<QLuaScript> mScript;
	LuaEditor* editor;
	QPushButton* btCompile;
};

#endif // QScriptEditor_h__
