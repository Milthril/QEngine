#ifndef QLua_h__
#define QLua_h__
#include "sol\sol.hpp"

#define Q_ENABLE_LUA public: \
	static void registerLua(sol::state&);

#endif // QLua_h__
