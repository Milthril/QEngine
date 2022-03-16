#ifndef QMaterialEditor_h__
#define QMaterialEditor_h__

#include "KDDockWidgets\DockWidget.h"

class QMaterial;
class GLSLEditor;
class UniformPanel;

class QMaterialEditor :public KDDockWidgets::DockWidget {
public:
	QMaterialEditor(std::shared_ptr<QMaterial> material = nullptr);
private:
	std::shared_ptr<QMaterial> mMaterial;
private:
	UniformPanel* unifrom;
	GLSLEditor* editor;
};

#endif // QMaterialEditor_h__
