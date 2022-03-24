#ifndef QLuaScript_h__
#define QLuaScript_h__

#include "QLocalInlineTypeDefine.h"
#include "QLuaEvent.h"

class QLuaScript {
	friend class QLuaScriptFactory;
public:
	enum Usgae {
		Uniform
	};
	QLuaScript(Usgae usage);
	void loadCode(const QByteArray& code);
	const QByteArray& getCode();
private:
	sol::state mLocalState;
	QLuaEvent mEvent;
	QByteArray mCode;
	Usgae mUsage;
};

#endif // QLuaScript_h__
