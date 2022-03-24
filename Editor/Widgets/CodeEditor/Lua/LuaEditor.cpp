#include "LuaEditor.h"
#include "Qsci\qscilexerlua.h"

LuaEditor::LuaEditor()
	:QCodeEditor(new QsciLexerLua)
{
	for (int i = 1; i <= 4; i++) {
		for (auto& keyword : QString(mLexer->keywords(i)).split(" "))
			mApis->add(keyword);
	}
}