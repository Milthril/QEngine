#include "QHBoxLayout"
#include "QMaterialEditor.h"
#include "QPushButton"
#include "QSplitter"
#include "Widgets\CodeEditor\GLSL\GLSLEditor.h"
#include "Widgets\CodeEditor\Lua\LuaEditor.h"
#include "Widgets\UniformPanel\UniformPanel.h"
#include "Script\QLuaScriptFactory.h"
#include "Asset\Material.h"

QMaterialEditor* QMaterialEditor::QMaterialEditor::instance(){
	static QMaterialEditor ins;
	return &ins;
}

void QMaterialEditor::edit(std::shared_ptr<Asset::Material> material)
{
	mMaterial = material;
	mUniformPanel->setUniform(std::dynamic_pointer_cast<QRhiUniform>(material));
	glslEditor->setText(mMaterial->getShadingCode());
	luaEditor->setText(mMaterial->getScript()->getCode());
	if (!isVisible())
		show();
	activateWindow();
	setFocus();
}

QMaterialEditor::QMaterialEditor()
	: KDDockWidgets::DockWidget("Material Editor")
	, luaEditor(new LuaEditor)
	, glslEditor(new GLSLEditor)
	, mUniformPanel(new UniformPanel())
	, btSetupShader(new QPushButton("Setup Material"))
	, btSetupLua(new QPushButton("Setup Script"))
{
	QsciAPIs* apis = luaEditor->getApis();
	for (auto& api : QLuaScriptFactory::instance()->generateAPIs(QLuaScript::Uniform)) {
		apis->add(api);
	}
	apis->prepare();

	QSplitter* MainPanel = new QSplitter;
	setWidget(MainPanel);
	MainPanel->addWidget(mUniformPanel);

	QSplitter* CodePanel = new QSplitter(Qt::Vertical);
	MainPanel->addWidget(CodePanel);

	QWidget* LuaPanel = new QWidget;
	QVBoxLayout* v = new QVBoxLayout(LuaPanel);
	v->addWidget(btSetupLua, 0, Qt::AlignRight);
	v->addWidget(luaEditor);
	CodePanel->addWidget(LuaPanel);

	QWidget* GlslPanel = new QWidget;
	v = new QVBoxLayout(GlslPanel);
	v->addWidget(btSetupShader, 0, Qt::AlignRight);
	v->addWidget(glslEditor);
	CodePanel->addWidget(GlslPanel);

	btSetupShader->setFixedSize(120, 25);
	btSetupLua->setFixedSize(120, 25);

	MainPanel->setStretchFactor(0, 2);
	MainPanel->setStretchFactor(1, 5);

	connect(btSetupLua, &QPushButton::clicked, this, [this]() {
		mMaterial->getScript()->loadCode(luaEditor->text().toLocal8Bit());
	});

	connect(btSetupShader, &QPushButton::clicked, this, [this]() {
		mMaterial->setShadingCode(glslEditor->text().toLocal8Bit());
	});
}