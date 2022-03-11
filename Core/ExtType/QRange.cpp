#include "QRange.h"

QRange::QRange(int lower, int upper, int min, int max)
	: lower_(lower)
	, upper_(upper)
	, min_(min)
	, max_(max)
{
}

void QRange::setLimite(int min, int max)
{
	min_ = min;
	max_ = max;
}

int QRange::min() const
{
	return min_;
}

void QRange::setMin(int min)
{
	min_ = min;
}

int QRange::max() const
{
	return max_;
}

void QRange::setMax(int max)
{
	max_ = max;
}

int QRange::getLower() const
{
	return lower_;
}

void QRange::setLower(int val){
	lower_ = std::clamp(val,min_,max_);
}

int QRange::getUpper() const
{
	return upper_;
}

void QRange::setUpper(int val){
	upper_ = std::clamp(val, min_, max_);
}

void QRange::setRange(int lower, int upper)
{
	lower_ = lower;
	upper_ = upper;
}

QDebug operator<<(QDebug debug, const QRange& range)
{
	debug <<"QRange(" << range.getLower()<< ", " << range.getUpper()<<")";
	return debug;
}

bool QRange::operator==(const QRange& other)
{
	return this->min() == other.min() &&
		this->max() == other.max() &&
		this->getLower() == other.getLower() &&
		this->getUpper() == other.getUpper();
}

QDataStream& operator<<(QDataStream& out, const QRange& var)
{
	out << var.lower_ << var.upper_ << var.min() << var.max();
	return out;
}

QDataStream& operator>>(QDataStream& in, QRange& var)
{
	in >> var.lower_ >> var.upper_ >> var.min_ >> var.max_;
	return in;
}