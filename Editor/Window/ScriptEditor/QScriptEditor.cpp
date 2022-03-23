#include "QScriptEditor.h"
#include "QHBoxLayout"
#include "Widgets\UniformPanel\UniformPanel.h"
#include "Script\QLuaScript.h"
#include "QSplitter"
#include "QPushButton"
#include "Widgets\CodeEditor\Lua\LuaEditor.h"

QScriptEditor* QScriptEditor::QScriptEditor::instance()
{
	static QScriptEditor ins;
	return &ins;
}

void QScriptEditor::edit(std::shared_ptr<QLuaScript> script) {
	mScript = script;
	editor->setText(mScript->getCode());
	if (!isVisible())
		show();
	activateWindow();
	setFocus();
}

QScriptEditor::QScriptEditor()
	: KDDockWidgets::DockWidget("Lua Script Editor")
	, editor(new LuaEditor)
	, btCompile(new QPushButton("Setup"))
{
	btCompile->setFixedSize(60, 25);

	QWidget* body = new QWidget;
	setWidget(body);

	QVBoxLayout* v = new QVBoxLayout(body);
	v->addWidget(btCompile, 0, Qt::AlignRight);
	v->addWidget(editor);

	connect(btCompile, &QPushButton::clicked, this, [this]() {
		mScript->loadCode(editor->text().toLocal8Bit());
	});
}