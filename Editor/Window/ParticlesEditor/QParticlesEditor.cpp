#include "QHBoxLayout"
#include "QParticlesEditor.h"
#include "QPushButton"
#include "QSplitter"
#include "Widgets\CodeEditor\GLSL\GLSLEditor.h"
#include "Widgets\UniformPanel\UniformPanel.h"
#include "Asset\PartcleSystem\Emitter\QParticleEmitter.h"
#include "Widgets\Inspector\QPropertyPanel.h"
#include "QApplication"

QParticlesEditor::QParticlesEditor(std::shared_ptr<Asset::ParticleSystem> system)
	: KDDockWidgets::DockWidget("Particles Editor")
	, mSystem(system)
	, editor(new GLSLEditor)
	, mUniformPanel(new UniformPanel())
	, btCompile(new QPushButton("Compile"))
	, mEmitterPanel(new QPropertyPanel)
{
	setAttribute(Qt::WA_DeleteOnClose);
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

	mUniformPanel->setUniform(std::dynamic_pointer_cast<QRhiUniform>(system->getUpdater()));
	mEmitterPanel->setObject(system->getEmitter().get());
	editor->setText(mSystem->getUpdater()->getUpdateCode());
	resize(1000, 700);
}


void QParticlesEditor::closeEvent(QCloseEvent*e)
{
	KDDockWidgets::DockWidget::closeEvent(e);
	mSystem.reset();
	mUniformPanel->setUniform(nullptr);
}
