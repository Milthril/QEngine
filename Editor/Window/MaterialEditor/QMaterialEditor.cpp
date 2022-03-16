#include "QMaterialEditor.h"
#include "QHBoxLayout"
#include "Scene\Material\QMaterial.h"
#include "Widgets\CodeEditor\GLSL\GLSLEditor.h"
#include "Widgets\UniformPanel\UniformPanel.h"
#include "QSplitter"

QMaterialEditor::QMaterialEditor(std::shared_ptr<QMaterial> material)
	: KDDockWidgets::DockWidget("Material Editor")
	, mMaterial(material)
	, editor(new GLSLEditor)
	, unifrom(new UniformPanel(std::dynamic_pointer_cast<QRhiUniform>(material)))
{
	QSplitter* body = new QSplitter;
	setWidget(body);
	body->addWidget(unifrom);
	body->addWidget(editor);
	editor->setText(material->getShadingCode());
}