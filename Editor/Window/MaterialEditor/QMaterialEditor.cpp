#include "QMaterialEditor.h"
#include "QHBoxLayout"
#include "Scene\Material\QMaterial.h"
#include "Widgets\CodeEditor\GLSL\GLSLEditor.h"
#include "Widgets\UniformPanel\UniformPanel.h"
#include "QSplitter"
#include "QPushButton"

QMaterialEditor* QMaterialEditor::QMaterialEditor::instance()
{
	static QMaterialEditor ins;
	return &ins;
}

void QMaterialEditor::edit(std::shared_ptr<QMaterial> material)
{
	mMaterial = material;
	mUniformPanel->setUniform(std::dynamic_pointer_cast<QRhiUniform>(material));
	editor->setText(mMaterial->getShadingCode());
	if (!isVisible())
		show();
	activateWindow();
	setFocus();
}

QMaterialEditor::QMaterialEditor()
	: KDDockWidgets::DockWidget("Particle Editor")
	, editor(new GLSLEditor)
	, mUniformPanel(new UniformPanel())
	, btCompile(new QPushButton("Compile"))
{
	QSplitter* body = new QSplitter;
	setWidget(body);
	body->addWidget(mUniformPanel);
	QWidget* right = new QWidget;
	QVBoxLayout* v = new QVBoxLayout(right);
	v->addWidget(btCompile, 0, Qt::AlignRight);
	btCompile->setFixedSize(60, 25);
	v->addWidget(editor);
	body->addWidget(right);
	body->setStretchFactor(0, 2);
	body->setStretchFactor(1, 5);
	connect(btCompile, &QPushButton::clicked, this, [this]() {
		mMaterial->setShadingCode(editor->text().toLocal8Bit());
	});
}