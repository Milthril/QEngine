#include "QPrimitiveComponent.h"

QVector4D operator<<(QVector4D& vec4, const QColor& color)
{
	vec4 = QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	return vec4;
}