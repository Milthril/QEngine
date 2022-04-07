#ifndef QMaterialEditor_h__
#define QMaterialEditor_h__

#include "KDDockWidgets\DockWidget.h"

class QMaterial;
class GLSLEditor;
class UniformPanel;
class QPushButton;
class LuaEditor;

class QMaterialEditor :public KDDockWidgets::DockWidget {
	Q_OBJECT
public:
	static QMaterialEditor* instance();
	void edit(std::shared_ptr<QMaterial> material);
private:
	QMaterialEditor();
private:
	std::shared_ptr<QMaterial> mMaterial;
private:
	UniformPanel* mUniformPanel;
	LuaEditor* luaEditor;
	GLSLEditor* glslEditor;

	QPushButton* btSetupShader;
	QPushButton* btSetupLua;
};

#endif // QMaterialEditor_h__
