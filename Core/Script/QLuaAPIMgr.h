#ifndef QLuaAPIMgr_h__
#define QLuaAPIMgr_h__

#include "QJsonObject"
#include "sol\state.hpp"

class QLuaAPIMgr {
public:
	static QLuaAPIMgr* instance();
	QStringList generateAPIs();
private:
	QLuaAPIMgr();
private:
	using ClassName = QString;
	using ClassInfo = QJsonObject;
	QHash<ClassName, ClassInfo> mMap;
};

#endif // QLuaAPIMgr_h__
