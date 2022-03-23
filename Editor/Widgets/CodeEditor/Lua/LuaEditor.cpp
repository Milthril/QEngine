#include "LuaEditor.h"
#include "Qsci\qscilexerlua.h"
#include "Script\QLuaAPIMgr.h"

LuaEditor::LuaEditor()
	:QCodeEditor(new QsciLexerLua)
{
	for (int i = 1; i <= 4; i++) {
		for (auto& keyword : QString(mLexer->keywords(i)).split(" "))
			mApis->add(keyword);
	}
	for (auto& api : QLuaAPIMgr::instance()->generateAPIs()) {
		mApis->add(api);
	}
	mApis->prepare();
}