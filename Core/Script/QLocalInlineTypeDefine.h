#ifndef QLocalInlineTypeDefine_h__
#define QLocalInlineTypeDefine_h__

#include <sol/sol.hpp>
#include <QString>
#include "ExtType/QBoundedInt.h"
#include "ExtType/QBoundedDouble.h"
#include "qvectornd.h"

template <typename Handler>
bool sol_lua_check(sol::types<QString>, lua_State* L,
				   int index, Handler&& handler,
				   sol::stack::record& tracking) {
	int absolute_index = lua_absindex(L, index);
	bool success = sol::stack::check<const char*>(
		L, absolute_index, handler);
	tracking.use(1);
	return success;
}

static QString sol_lua_get(sol::types<QString>, lua_State* L, int index, sol::stack::record& tracking) {
	int absolute_index = lua_absindex(L, index);
	std::string  str = sol::stack::get<const char*>(L, absolute_index);
	tracking.use(1);
	return QString{ QString::fromStdString(str) };
}

static int sol_lua_push(lua_State* L, const QString& str) {
	lua_pushlstring(L, str.toLocal8Bit().constData(), str.size());;
	return 1;
}

namespace sol {
template <>
struct lua_type_of<QString>
	: std::integral_constant<type, type::string> { };
} // namespace sol

template <typename Handler>
bool sol_lua_check(sol::types<QBoundedInt>, lua_State* L,
				   int index, Handler&& handler,
				   sol::stack::record& tracking) {
	int absolute_index = lua_absindex(L, index);
	bool success = sol::stack::check<int>(
		L, absolute_index, handler);
	tracking.use(1);
	return success;
}

static QBoundedInt sol_lua_get(sol::types<QBoundedInt>, lua_State* L, int index, sol::stack::record& tracking) {
	int absolute_index = lua_absindex(L, index);
	int value = sol::stack::get<int>(L, absolute_index);
	tracking.use(1);
	return QBoundedInt(value);
}

static int sol_lua_push(lua_State* L, const QBoundedInt& value) {
	lua_pushnumber(L, value.number());
	return 1;
}

namespace sol {
template <>
struct lua_type_of<QBoundedInt>
	: std::integral_constant<type, type::number> { };
} // namespace sol

template <typename Handler>
bool sol_lua_check(sol::types<QBoundedDouble>, lua_State* L,
				   int index, Handler&& handler,
				   sol::stack::record& tracking) {
	int absolute_index = lua_absindex(L, index);
	bool success = sol::stack::check<double>(
		L, absolute_index, handler);
	tracking.use(1);
	return success;
}

static QBoundedDouble sol_lua_get(sol::types<QBoundedDouble>, lua_State* L, int index, sol::stack::record& tracking) {
	int absolute_index = lua_absindex(L, index);
	double value = sol::stack::get<double>(L, absolute_index);
	tracking.use(1);
	return QBoundedDouble(value);
}

static int sol_lua_push(lua_State* L, const QBoundedDouble& value) {
	lua_pushnumber(L, value.number());
	return 1;
}

namespace sol {
template <>
struct lua_type_of<QBoundedDouble>
	: std::integral_constant<type, type::number> { };
} // namespace sol

#endif // QLocalInlineTypeDefine_h__
