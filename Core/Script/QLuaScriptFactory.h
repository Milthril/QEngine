#ifndef QLuaScriptFactory_h__
#define QLuaScriptFactory_h__

#include "QJsonObject"
#include "sol\state.hpp"
#include "QLuaScript.h"

class QRhiUniform;

class QLuaScriptFactory {
public:
	static QLuaScriptFactory* instance();
	std::shared_ptr<QLuaScript> createUniformScript(QRhiUniform* uniform);
	QStringList generateAPIs(QLuaScript::Usgae usage);
private:
	QLuaScriptFactory();
private:
	using ClassName = QString;
	using ClassInfo = QJsonObject;
	QHash<ClassName, ClassInfo> mMap;
};

#endif // QLuaScriptFactory_h__
