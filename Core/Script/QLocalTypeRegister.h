#ifndef QLocalTypeRegister_h__
#define QLocalTypeRegister_h__

#include <sol/sol.hpp>
#include "qvectornd.h"

#define TypeToString(Type) sol::meta_function::to_string, [](const Type& var) {return QDebug::toString(var).toStdString();}

template<typename Type>
void QLuaRegister(sol::state& lua) {
};

template<typename Type>
QStringList QLocalLuaAPI() {
	return {};
};

template<>
void QLuaRegister<QRhiUniform>(sol::state& lua) {
	lua.new_usertype<QRhiUniform>("QRhiUniform"
								  , "setData", sol::overload(&QRhiUniform::setDataFloat, &QRhiUniform::setDataVec2, &QRhiUniform::setDataVec3, &QRhiUniform::setDataVec4)
								  , "getData", sol::overload(&QRhiUniform::getData<float>, &QRhiUniform::getData<QVector2D>, &QRhiUniform::getData<QVector3D>, &QRhiUniform::getData<QVector4D>)
								  );
}
template<>
QStringList QLocalLuaAPI<QRhiUniform>() {
	return {
		"UBO.setData(QString name ,float var)",
		"UBO.setData(QString name ,QVector2D var)",
		"UBO.setData(QString name ,QVector3D var)",
		"UBO.setData(QString name ,QVector4D var)",
		"UBO.getData(QString name) -> float",
		"UBO.getData(QString name) -> QVector2D",
		"UBO.getData(QString name) -> QVector3D",
		"UBO.getData(QString name) -> QVector4",
	};
};

template<>
void QLuaRegister<QVector2D>(sol::state& lua) {
	lua.new_usertype<QVector2D>("QVector2D"
								, sol::constructors<QVector2D(float, float)>()
								, "x", sol::property(&QVector2D::x, &QVector2D::setX)
								, "y", sol::property(&QVector2D::y, &QVector2D::setY)
								, TypeToString(QVector2D)
								);
}
template<>
QStringList QLocalLuaAPI<QVector2D>() {
	return {
		"QVector2D.new(float x,float y)",
		"QVector2D.x",
		"QVector2D.y",
	};
};

template<>
void QLuaRegister<QVector3D>(sol::state& lua) {
	lua.new_usertype<QVector3D>("QVector3D"
								, sol::constructors<QVector3D(float, float, float)>()
								, "x", sol::property(&QVector3D::x, &QVector3D::setX)
								, "y", sol::property(&QVector3D::y, &QVector3D::setY)
								, "z", sol::property(&QVector3D::z, &QVector3D::setZ)
								, TypeToString(QVector3D)
								);
}
template<>
QStringList QLocalLuaAPI<QVector3D>() {
	return {
		"QVector3D.new(float x,float y,float z)",
		"QVector3D.x",
		"QVector3D.y",
		"QVector3D.z",
	};
};

template<>
void QLuaRegister<QVector4D>(sol::state& lua) {
	lua.new_usertype<QVector4D>("QVector4D"
								, sol::constructors<QVector4D(float, float, float, float)>()
								, "x", sol::property(&QVector4D::x, &QVector4D::setX)
								, "y", sol::property(&QVector4D::y, &QVector4D::setY)
								, "z", sol::property(&QVector4D::z, &QVector4D::setZ)
								, "w", sol::property(&QVector4D::w, &QVector4D::setW)
								, TypeToString(QVector4D)
								);
}

template<>
QStringList QLocalLuaAPI<QVector4D>() {
	return {
		"QVector4D.new(float x,float y,float z,float w)",
		"QVector4D.x",
		"QVector4D.y",
		"QVector4D.z",
		"QVector4D.w",
	};
};

template<>
void QLuaRegister<QColor>(sol::state& lua) {
	lua.new_usertype<QColor>("QColor"
							 , "red", sol::property(&QColor::red, &QColor::setRed)
							 , "green", sol::property(&QColor::green, &QColor::setGreen)
							 , "blue", sol::property(&QColor::blue, &QColor::setBlue)
							 , "alpha", sol::property(&QColor::alpha, &QColor::setAlpha)
							 , TypeToString(QColor)
							 );
}

template<>
QStringList QLocalLuaAPI<QColor>() {
	return {
		"QColor.new(float r,float g,float b,float a)",
		"QColor.red",
		"QColor.green",
		"QColor.blue",
		"QColor.alpha",
	};
};

//struct RangeStaticRegister : public LuaRegisterBase {
//	void typeToLua(sol::state& lua) override {
//		lua.new_usertype<QRange>("QRange"
//								 , "lower", sol::property(&QRange::getLower, &QRange::setLower)
//								 , "upper", sol::property(&QRange::getUpper, &QRange::setUpper)
//								 , TypeToString(QRange)
//								 );
//	}
//	std::vector<propertyInfo> propertyDescription() override {
//		return { {"float","lower"},
//				{"float","upper"},
//		};
//	}
//	void objectToLua(void* object, std::string varName, sol::state& lua) override {
//		lua[varName] = static_cast<QRange*>(object);
//	}
//};
//
//struct ComboStaticRegister : public LuaRegisterBase {
//	void typeToLua(sol::state& lua) override {
//		lua.new_usertype<QCombo>("QCombo"
//								 , "currentIndex", sol::property(&QCombo::getCurrentIndex, &QCombo::setCurrentIndex)
//								 , "currentItem", sol::property(&QCombo::getCurrentItem, &QCombo::setCurrentItem)
//								 , TypeToString(QCombo)
//								 );
//	}
//	std::vector<propertyInfo> propertyDescription() override {
//		return { {"int","currentIndex"},
//				 {"QString","currentItem"},
//		};
//	}
//	void objectToLua(void* object, std::string varName, sol::state& lua) override {
//		lua[varName] = static_cast<QCombo*>(object);
//	}
//};
//

#endif // QLocalTypeRegister_h__