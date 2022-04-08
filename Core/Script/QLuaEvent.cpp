#include "QLuaEvent.h"

void QLuaEvent::tickEvent(float deltaSeconds)
{
	if (mTickEvent) {
		mTickEvent();
	}
}