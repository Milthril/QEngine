#include "QLuaEvent.h"

void QLuaEvent::tickEvent()
{
	if (mTickEvent) {
		mTickEvent();
	}
}