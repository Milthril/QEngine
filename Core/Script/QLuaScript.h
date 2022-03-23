#ifndef QLuaScript_h__
#define QLuaScript_h__

#include "sol\sol.hpp"
#include "QLuaEvent.h"

class QLuaScript {
public:
	QLuaScript();
	void loadCode(const QByteArray& code);
	const QByteArray& getCode();

private:
	sol::state mLocalState;
	std::shared_ptr<QLuaEvent> mEvent;
	QByteArray mCode;
};

#endif // QLuaScript_h__
