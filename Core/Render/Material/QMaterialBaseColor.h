#ifndef QMaterialBaseColor_h__
#define QMaterialBaseColor_h__

#include "QMaterial.h"

class QMaterialBaseColor :public QMaterial {
public:
	QMaterialBaseColor();
	QVector4D getColor();
	void setColor(QVector4D color);
};

#endif // QMaterialBaseColor_h__