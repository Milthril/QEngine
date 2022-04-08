#ifndef QLuaEvent_h__
#define QLuaEvent_h__

#include "QObject"
#include "QLua.h"
#include "EventHandler\QTickEventHandler.h"

class QLuaEvent :public QObject, public QTickEventHandler {
	Q_OBJECT
		Q_ENABLE_LUA
		Q_PROPERTY(sol::function Tick READ getTickEvent WRITE setTickEvent)
public:
	const sol::function& getTickEvent() const { return mTickEvent; }
	void setTickEvent(const sol::function& val) { mTickEvent = val; }
	sol::function mTickEvent;
protected:
	virtual void tickEvent(float deltaSeconds) override;
};

#endif // QLuaEvent_h__
