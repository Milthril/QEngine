#include "Vec4Box.h"
#include <QHBoxLayout>
#include "DoubleBox.h"
#include "Toolkit/QNeumorphism.h"

Vec4Box::Vec4Box(QVector4D value, QWidget* parent /*= nullptr*/)
	: x(new DoubleBox(value.x(), "X"))
	, y(new DoubleBox(value.y(), "Y"))
	, z(new DoubleBox(value.z(), "Z"))
	, w(new DoubleBox(value.w(), "W"))
{
	x->setGraphicsEffect(nullptr);
	y->setGraphicsEffect(nullptr);
	z->setGraphicsEffect(nullptr);
	w->setGraphicsEffect(nullptr);
	setGraphicsEffect(new QNeumorphism);

	QVBoxLayout* h = new QVBoxLayout(this);
	h->setSpacing(2);
	h->setContentsMargins(0, 0, 0, 0);
	h->addWidget(x);
	h->addWidget(y);
	h->addWidget(z);
	h->addWidget(w);
	connect(x, &Adjuster::valueChanged, this, &Vec4Box::emitSignal);
	connect(y, &Adjuster::valueChanged, this, &Vec4Box::emitSignal);
	connect(w, &Adjuster::valueChanged, this, &Vec4Box::emitSignal);
	connect(z, &Adjuster::valueChanged, this, &Vec4Box::emitSignal);
}

QVariant Vec4Box::getValue()
{
	return QVector4D(x->number(), y->number(), z->number(), w->number());
}

void Vec4Box::setValue(QVariant var)
{
	QVector4D vec = var.value<QVector4D>();
	singalLocker = true;
	x->setNumber(vec.x());
	y->setNumber(vec.y());
	z->setNumber(vec.z());
	singalLocker = false;

	w->setNumber(vec.w());
}

void Vec4Box::emitSignal(QVariant)
{
	if (singalLocker)
		return;
	Q_EMIT valueChanged(getValue());
}