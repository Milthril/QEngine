#include "QMaterialBaseColor.h"

QMaterialBaseColor::QMaterialBaseColor(QVector4D color)
{
	addDataVec4("BaseColor", color);
	setShadingCode("FragColor = material.BaseColor;");
}

QVector4D QMaterialBaseColor::getColor()
{
	return getData<QVector4D>("BaseColor");
}

void QMaterialBaseColor::setColor(QVector4D color)
{
	setData<QVector4D>("BaseColor", color);
}