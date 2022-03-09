#include "QMaterialBaseColor.h"

QMaterialBaseColor::QMaterialBaseColor(QVector4D color)
{
	addParamVec4("BaseColor", color);
	setShadingCode("FragColor = material.BaseColor;");
}

QVector4D QMaterialBaseColor::getColor()
{
	return getParam<QVector4D>("BaseColor");
}

void QMaterialBaseColor::setColor(QVector4D color)
{
	setParam<QVector4D>("BaseColor", color);
}