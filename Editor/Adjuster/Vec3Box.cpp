#include "Vec3Box.h"
#include <QHBoxLayout>
#include "DoubleBox.h"
#include "Toolkit/QNeumorphism.h"

Vec3Box::Vec3Box(QVector3D value, QWidget* parent /*= nullptr*/)
	: x(new DoubleBox(value.x(), "X"))
	, y(new DoubleBox(value.y(), "Y"))
	, z(new DoubleBox(value.z(), "Z"))
{
	x->setGraphicsEffect(nullptr);
	y->setGraphicsEffect(nullptr);
	z->setGraphicsEffect(nullptr);
	setGraphicsEffect(new QNeumorphism);
	QVBoxLayout* h = new QVBoxLayout(this);
	h->setSpacing(2);
	h->setContentsMargins(0, 0, 0, 0);
	h->addWidget(x);
	h->addWidget(y);
	h->addWidget(z);
	connect(x, &Adjuster::valueChanged, this, &Vec3Box::emitSignal);
	connect(y, &Adjuster::valueChanged, this, &Vec3Box::emitSignal);
	connect(z, &Adjuster::valueChanged, this, &Vec3Box::emitSignal);
}

QVariant Vec3Box::getValue()
{
	return QVector3D(x->number(), y->number(), z->number());
}

void Vec3Box::setValue(QVariant var)
{
	QVector3D vec = var.value<QVector3D>();
	singalLocker = true;
	x->setNumber(vec.x());
	y->setNumber(vec.y());
	singalLocker = false;
	z->setNumber(vec.z());
}

void Vec3Box::emitSignal(QVariant)
{
	if (singalLocker)
		return;
	Q_EMIT valueChanged(getValue());
}