#include "QLuaScriptFactory.h"
#include "QDir"
#include "QJsonDocument"
#include "QJsonArray"
#include "Scene\Component\Particle\PositionGenerator\QCubeGenerator.h"
#include "QQueue"
#include "RHI\QRhiUniform.h"
#include "QLocalTypeRegister.h"

QLuaScriptFactory* QLuaScriptFactory::instance()
{
	static QLuaScriptFactory ins;
	return &ins;
}

std::shared_ptr<QLuaScript> QLuaScriptFactory::createUniformScript(QRhiUniform* uniform)
{
	std::shared_ptr<QLuaScript> script = std::make_shared<QLuaScript>(QLuaScript::Uniform);
	QLuaEvent::registerLua(script->mLocalState);
	QLuaRegister<QRhiUniform>(script->mLocalState);
	QLuaRegister<QVector2D>(script->mLocalState);
	QLuaRegister<QVector3D>(script->mLocalState);
	QLuaRegister<QVector4D>(script->mLocalState);
	script->mLocalState["UBO"] = uniform;
	return script;
}

QStringList QLuaScriptFactory::generateAPIs(QLuaScript::Usgae usage) {
	QStringList apis;
	for (auto& cls : mMap) {
		QString classname = cls["ClassName"].toString();
		QJsonObject properties = cls["Properties"].toObject();
		for (auto& prop : properties.keys()) {
			apis << classname + "." + prop;
		}
		QJsonObject functions = cls["Functions"].toObject();
		for (auto& funcName : functions.keys()) {
			QString funcDefine = funcName + "(";
			QJsonObject funcInfo = functions[funcName].toObject();
			bool flag = false;
			if (!funcInfo["isStatic"].toBool()) {
				funcDefine += "self";
				flag = true;
			}
			QJsonArray args = funcInfo["Arguments"].toArray();
			for (auto arg : args) {
				QJsonObject argInfo = arg.toObject();
				if (flag)
					funcDefine += " ,";
				else
					flag = true;
				funcDefine += argInfo["Type"].toString() + " " + argInfo["Name"].toString();
			}
			funcDefine += ")";
			apis << classname + "." + funcDefine;
		}
	}
	apis << "Event.Tick";
	apis << QLocalLuaAPI<QRhiUniform>();
	return apis;
}

QLuaScriptFactory::QLuaScriptFactory()
{
	QDir dir(LUA_API_DIR);
	for (auto& info : dir.entryInfoList(QDir::Filter::Files)) {
		QFile file(info.filePath());
		if (file.open(QFile::ReadOnly)) {
			QJsonObject classInfo = QJsonDocument::fromJson(file.readAll()).object();
			QString className = classInfo["ClassName"].toString();
			if (!className.isEmpty()) {
				mMap[className] = classInfo;
			}
		}
	}
}