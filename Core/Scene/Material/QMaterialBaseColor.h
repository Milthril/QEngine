#ifndef QMaterialBaseColor_h__
#define QMaterialBaseColor_h__

#include "QMaterial.h"

class QMaterialBaseColor :public QMaterial {
public:
	QMaterialBaseColor(QVector4D color = QVector4D(1,1,1,1));
	QVector4D getColor();
	void setColor(QVector4D color);
};

#endif // QMaterialBaseColor_h__