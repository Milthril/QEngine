#include "QBoundedDouble.h"

QBoundedDouble::QBoundedDouble(double number, double min, double max) : value_(number)
, min_(min)
, max_(max)
{
}

QBoundedDouble::QBoundedDouble(const QBoundedDouble& other)
{
	if (other.max() != other.min()) {
		this->max_ = other.max_;
		this->min_ = other.min_;
	}
	if (this->max_ == 0 && this->min_ == 0) {
		this->value_ = other.value_;
	}
	else {
		setNumber(other.value_);
	}
}

void QBoundedDouble::setBound(double min, double max)
{
	min_ = min;
	max_ = max;
}

double QBoundedDouble::min() const
{
	return min_;
}

void QBoundedDouble::setMin(double min)
{
	min_ = min;
}

double QBoundedDouble::max() const
{
	return max_;
}

void QBoundedDouble::setMax(double max)
{
	max_ = max;
}

double QBoundedDouble::number() const
{
	return value_;
}

bool QBoundedDouble::setNumber(double number)
{
	number = qBound(min_, number, max_);
	if (number == value_)
		return false;
	value_ = number;
	return true;
}

bool QBoundedDouble::operator==(const QBoundedDouble& other)
{
	return this->number() == other.number();
}

QDataStream& operator<<(QDataStream& out, const QBoundedDouble& var)
{
	out << var.number() << var.min() << var.max();
	return out;
}

QDataStream& operator>>(QDataStream& in, QBoundedDouble& var)
{
	in >> var.value_ >> var.min_ >> var.max_;
	return in;
}