#ifndef Vec4Box_h__
#define Vec4Box_h__

#include "Adjuster.h"
#include <QVector4D>

class DoubleBox;

class Vec4Box : public Adjuster
{
	Q_OBJECT
public:
	Vec4Box(QVector4D value, QWidget* parent = nullptr);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	DoubleBox* x = nullptr;
	DoubleBox* y = nullptr;
	DoubleBox* z = nullptr;
	DoubleBox* w = nullptr;
private:
	void emitSignal(QVariant);
	bool singalLocker = false;
};

#endif // Vec4Box_h__
