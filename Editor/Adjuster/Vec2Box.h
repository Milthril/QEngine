#ifndef Vec2Box_h__
#define Vec2Box_h__

#include "Adjuster.h"
#include <QVector2D>

class DoubleBox;

class Vec2Box : public Adjuster
{
	Q_OBJECT
public:
	Vec2Box(QVector2D value, QWidget* parent = nullptr);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	DoubleBox* x = nullptr;
	DoubleBox* y = nullptr;
private:
	void emitSignal(QVariant);
	bool singalLocker = false;
};

#endif // Vec2Box_h__
