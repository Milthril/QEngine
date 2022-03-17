#ifndef QMaterial_h__
#define QMaterial_h__

#include "RHI\QRhiUniform.h"

class QPrimitiveComponent;

class QMaterial :public QRhiUniform {
	friend class QMaterialProxy;
public:
	QMaterial();
	QByteArray getShadingCode() const { return mShadingCode; }
	void setShadingCode(QByteArray val);

protected:
	QByteArray mShadingCode;
};

Q_DECLARE_METATYPE(std::shared_ptr<QMaterial>)
#endif // QMaterial_h__
