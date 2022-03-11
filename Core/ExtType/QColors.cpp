#include "QColors.h"

QColors::QColors()
{
	stops_.push_back({ 0.5,Qt::white });
}

QColors::QColors(QGradientStops stops)
	:stops_(stops)
{
}

QGradientStops QColors::getStops() const
{
	return stops_;
}

void QColors::setStops(QGradientStops val)
{
	stops_ = val;
}

QDataStream& operator>>(QDataStream& in, QColors& var)
{
	in >> var.stops_;
	return in;
}

QDataStream& operator<<(QDataStream& out, const QColors& var)
{
	out << var.stops_;
	return out;
}

bool QColors::operator==(const QColors& other)
{
	return other.stops_ == this->stops_;
}