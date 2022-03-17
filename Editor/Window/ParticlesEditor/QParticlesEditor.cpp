#include "QHBoxLayout"
#include "QParticlesEditor.h"
#include "QPushButton"
#include "QSplitter"
#include "Widgets\CodeEditor\GLSL\GLSLEditor.h"
#include "Widgets\UniformPanel\UniformPanel.h"
#include "Scene\Component\Particle\QParticleSystem.h"

QParticlesEditor* QParticlesEditor::QParticlesEditor::instance()
{
	static QParticlesEditor ins;
	return &ins;
}

void QParticlesEditor::edit(std::shared_ptr<QParticleSystem> system)
{
	mSystem = system;
	mUniformPanel->setUniform(std::dynamic_pointer_cast<QRhiUniform>(system->getUpdater()));
	editor->setText(mSystem->getUpdater()->getUpdateCode());
	if (!isVisible())
		show();
}

QParticlesEditor::QParticlesEditor()
	: KDDockWidgets::DockWidget("Material Editor")
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
	btCompile->setFixedSize(60, 30);
	v->addWidget(editor);
	body->addWidget(right);
	body->setStretchFactor(0, 2);
	body->setStretchFactor(1, 5);
	connect(btCompile, &QPushButton::clicked, this, [this]() {
		mSystem->getUpdater()->setUpdateCode(editor->text().toLocal8Bit());
	});
}