#ifndef QMaterialEditor_h__
#define QMaterialEditor_h__

#include "KDDockWidgets\DockWidget.h"

class QMaterial;
class GLSLEditor;
class UniformPanel;
class QPushButton;

class QMaterialEditor :public KDDockWidgets::DockWidget {
public:
	static QMaterialEditor* instance();
	void edit(std::shared_ptr<QMaterial> material);
private:
	QMaterialEditor();
private:
	std::shared_ptr<QMaterial> mMaterial;
private:
	UniformPanel* mUniformPanel;
	GLSLEditor* editor;
	QPushButton* btCompile;
};

#endif // QMaterialEditor_h__
