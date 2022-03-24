#include "QLuaScript.h"
#include "Scene\Component\Particle\PositionGenerator\QCubeGenerator.h"

QLuaScript::QLuaScript(Usgae usage)
	:mUsage(usage)
{
	mLocalState.open_libraries();
	QCubeGenerator::registerLua(mLocalState);
	mLocalState["Event"] = &mEvent;
}

void QLuaScript::loadCode(const QByteArray& code)
{
	mCode = code;
	sol::protected_function_result ret = mLocalState.do_string(code.data());
	if (!ret.valid()) {
		sol::error err = ret;
		qDebug() << err.what();
	}
}

const QByteArray& QLuaScript::getCode()
{
	return mCode;
}