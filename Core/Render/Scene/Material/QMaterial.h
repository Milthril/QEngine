#ifndef QMaterial_h__
#define QMaterial_h__

#include "Render\RHI\QRhiUniform.h"

class QMaterial :public QRhiUniform {
	friend class QMaterialProxy;
public:
	QMaterial();
	QByteArray getShadingCode() const { return mShadingCode; }
	void setShadingCode(QByteArray val) { mShadingCode = val; }
protected:
	QByteArray mShadingCode;
};

#endif // QMaterial_h__
