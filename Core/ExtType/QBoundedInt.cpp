#include "QBoundedInt.h"

QBoundedInt::QBoundedInt(int number, int min, int max) : value_(number)
, min_(min)
, max_(max)
{
}


QBoundedInt::QBoundedInt(const QBoundedInt & other)
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

void QBoundedInt::setBound(int min, int max)
{
	min_ = min;
	max_ = max;
}

int QBoundedInt::min() const
{
	return min_;
}

void QBoundedInt::setMin(int min)
{
	min_ = min;
}

int QBoundedInt::max() const
{
	return max_;
}

void QBoundedInt::setMax(int max)
{
	max_ = max;
}

int QBoundedInt::number() const
{
	return value_;
}

bool QBoundedInt::setNumber(int number)
{
	number = qBound(min_, number, max_);
	if (number == value_)
		return false;
	value_ = number;
	return true;
}

bool QBoundedInt::operator==(const QBoundedInt& other)
{
	return this->number() == other.number();
}

QDataStream& operator<<(QDataStream& out, const QBoundedInt& var)
{
	out << var.number() << var.min() << var.max();
	return out;
}

QDataStream& operator>>(QDataStream& in, QBoundedInt& var)
{
	in >> var.value_ >> var.min_ >> var.max_;
	return in;
}