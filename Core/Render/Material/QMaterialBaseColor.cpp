#include "QMaterialBaseColor.h"

QMaterialBaseColor::QMaterialBaseColor()
{
	addParamVec4("BaseColor", QVector4D(1, 1, 1, 1));
	setShadingCode("FragColor = BaseColor;");
}

QVector4D QMaterialBaseColor::getColor()
{
	return getParam<QVector4D>("BaseColor");
}

void QMaterialBaseColor::setColor(QVector4D color)
{
	setParam<QVector4D>("BaseColor", color);
}
