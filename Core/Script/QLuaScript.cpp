#include "QLuaScript.h"
#include <QDebug>

QLuaScript::QLuaScript(Usgae usage)
	:mUsage(usage)
{
	mLocalState.open_libraries();
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