#include "QLuaScript.h"
#include "QLuaAPIMgr.h"
#include "Scene\Component\Particle\PositionGenerator\QCubeGenerator.h"

QLuaScript::QLuaScript()
	:mEvent(std::make_shared<QLuaEvent>())
{
	mLocalState.open_libraries();
	QCubeGenerator::registerLua(mLocalState);
	QLuaEvent::registerLua(mLocalState);
	mLocalState["Event"] = mEvent;
}

void QLuaScript::loadCode(const QByteArray& code)
{
	mCode = code;
	mLocalState.do_string(code.data());
}

const QByteArray& QLuaScript::getCode()
{
	return mCode;
}