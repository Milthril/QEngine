#include "QHBoxLayout"
#include "QParticlesEditor.h"
#include "QPushButton"
#include "QSplitter"
#include "Widgets\CodeEditor\GLSL\GLSLEditor.h"
#include "Widgets\UniformPanel\UniformPanel.h"
#include "Widgets\PropertyPanel\QPropertyPanel.h"
#include "Asset\PartcleSystem\Emitter\QParticleEmitter.h"

QParticlesEditor* QParticlesEditor::QParticlesEditor::instance()
{
	static QParticlesEditor ins;
	return &ins;
}

void QParticlesEditor::edit(std::shared_ptr<Asset::ParticleSystem> system)
{
	mSystem = system;
	mUniformPanel->setUniform(std::dynamic_pointer_cast<QRhiUniform>(system->getUpdater()));
	mEmitterPanel->setObject(system->getEmitter().get());
	editor->setText(mSystem->getUpdater()->getUpdateCode());
	if (!isVisible())
		show();
	activateWindow();
	setFocus();
}

QParticlesEditor::QParticlesEditor()
	: KDDockWidgets::DockWidget("Particles Editor")
	, editor(new GLSLEditor)
	, mUniformPanel(new UniformPanel())
	, btCompile(new QPushButton("Compile"))
	, mEmitterPanel(new QPropertyPanel)
{
	QSplitter* body = new QSplitter;
	setWidget(body);
	QSplitter* leftPanel = new QSplitter;
	leftPanel->setOrientation(Qt::Orientation::Vertical);
	leftPanel->addWidget(mEmitterPanel);
	leftPanel->addWidget(mUniformPanel);
	body->addWidget(leftPanel);
	QWidget* right = new QWidget;
	QVBoxLayout* v = new QVBoxLayout(right);
	v->addWidget(btCompile, 0, Qt::AlignRight);
	btCompile->setFixedSize(60, 25);
	v->addWidget(editor);
	body->addWidget(right);
	body->setStretchFactor(0, 2);
	body->setStretchFactor(1, 5);
	connect(btCompile, &QPushButton::clicked, this, [this]() {
		mSystem->getUpdater()->setUpdateCode(editor->text().toLocal8Bit());
	});
}

void QParticlesEditor::closeEvent(QCloseEvent*e) {
	mSystem.reset();
	KDDockWidgets::DockWidget::closeEvent(e);
}
