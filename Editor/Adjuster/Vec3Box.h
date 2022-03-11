#ifndef Vec3Box_h__
#define Vec3Box_h__

#include "Adjuster.h"
#include <QVector3D>

class DoubleBox;

class Vec3Box : public Adjuster
{
	Q_OBJECT
public:
	Vec3Box(QVector3D value, QWidget* parent = nullptr);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	DoubleBox* x = nullptr;
	DoubleBox* y = nullptr;
	DoubleBox* z = nullptr;
private:
	void emitSignal(QVariant);
	bool singalLocker = false;
};

#endif // Vec3Box_h__
