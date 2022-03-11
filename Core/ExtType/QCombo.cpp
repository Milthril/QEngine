#include "QCombo.h"
#include <QDataStream>

QCombo::QCombo(int currentIndex, QStringList items)
	: currentIndex_(currentIndex)
	, items_(items)
{
}

QCombo::QCombo()
{
}

QCombo::QCombo(QString currentItem, QStringList items)
	:currentIndex_(items.indexOf(currentItem))
	, items_(items)
{
}

void QCombo::clearItem()
{
	items_.clear();
}

void QCombo::addItem(QString item)
{
	items_ << item;
}

void QCombo::setComboList(QStringList list)
{
	items_ = list;
}

int QCombo::getCurrentIndex() const
{
	return currentIndex_;
}

void QCombo::setCurrentIndex(int val)
{
	if (items_.isEmpty()) {
		currentIndex_ = val;
	}
	else {
		currentIndex_ = std::clamp(val, 0, (int)items_.size() - 1);
	}
}

QString QCombo::getCurrentItem() const
{
	return items_.value(currentIndex_);
}

void QCombo::setCurrentItem(QString str)
{
	setCurrentIndex(items_.indexOf(str));
}

QDebug operator<<(QDebug debug, const QCombo& combo)
{
	debug << "QCombo{ " << "current index : " << combo.getCurrentIndex() <<", current item : " << combo.getCurrentItem()<<  " }";
	return debug;
}

bool QCombo::operator==(const QCombo& other)
{
	return getCurrentIndex() == other.getCurrentIndex() && items_ == other.items_;
}

QDataStream& operator>>(QDataStream& in, QCombo& var)
{
	in >> var.currentIndex_ >> var.items_;
	return in;
}
QDataStream& operator<<(QDataStream& out, const QCombo& var)
{
	out << var.currentIndex_ << var.items_;
	return out;
}