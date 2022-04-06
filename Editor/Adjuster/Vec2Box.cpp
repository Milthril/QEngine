#include "Vec2Box.h"
#include <QHBoxLayout>
#include "DoubleBox.h"

Vec2Box::Vec2Box(QVector2D value, QWidget* parent /*= nullptr*/)
	: x(new DoubleBox(value.x(), "X"))
	, y(new DoubleBox(value.y(), "Y"))
{
	x->setGraphicsEffect(nullptr);
	y->setGraphicsEffect(nullptr);
	QVBoxLayout* h = new QVBoxLayout(this);
	h->setSpacing(2);
	h->setContentsMargins(0, 0, 0, 0);
	h->addWidget(x);
	h->addWidget(y);
	connect(x, &Adjuster::valueChanged, this, &Vec2Box::emitSignal);
	connect(y, &Adjuster::valueChanged, this, &Vec2Box::emitSignal);
}

QVariant Vec2Box::getValue()
{
	return QVector2D(x->number(), y->number());
}

void Vec2Box::setValue(QVariant var)
{
	QVector2D vec = var.value<QVector2D>();
	singalLocker = true;
	x->setNumber(vec.x());
	singalLocker = false;
	y->setNumber(vec.y());
}

void Vec2Box::emitSignal(QVariant)
{
	if (singalLocker)
		return;
	Q_EMIT valueChanged(getValue());
}