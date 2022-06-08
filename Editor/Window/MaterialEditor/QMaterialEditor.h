#ifndef QMaterialEditor_h__
#define QMaterialEditor_h__

#include "KDDockWidgets\DockWidget.h"

namespace Asset{
class Material;
}
class GLSLEditor;
class UniformPanel;
class QPushButton;
class LuaEditor;

class QMaterialEditor :public KDDockWidgets::DockWidget {
	Q_OBJECT
public:
	QMaterialEditor(std::shared_ptr<Asset::Material> material);
protected:
	void closeEvent(QCloseEvent*) override;
private:
	std::shared_ptr<Asset::Material> mMaterial;

	UniformPanel* mUniformPanel;
	LuaEditor* luaEditor;
	GLSLEditor* glslEditor;

	QPushButton* btSetupShader;
	QPushButton* btSetupLua;
};

#endif // QMaterialEditor_h__
