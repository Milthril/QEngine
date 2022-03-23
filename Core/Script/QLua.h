#ifndef QLua_h__
#define QLua_h__

#include "sol\sol.hpp"
#include "QObject"
#include "EventHandler\QTickEventHandler.h"

#define Q_ENABLE_LUA static void registerLua(sol::state&);

class QLuaScript :public QObject, public QTickEventHandler {
	Q_OBJECT
		Q_ENABLE_LUA
		Q_PROPERTY(sol::function EventTick READ getEventTick WRITE setEventTick)
public:
	const sol::function& getEventTick() const { return mEventTick; }
	void setEventTick(sol::function val) { mEventTick = val; }
protected:
	void tickEvent() override;
private:
	sol::function mEventTick;
};

#endif // QLua_h__
