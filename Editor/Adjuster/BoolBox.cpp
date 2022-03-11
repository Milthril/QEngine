#include "BoolBox.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

BoolBox::BoolBox(bool value/*= 0*/, QWidget* parent /*= nullptr*/)
	: Button(parent)
	, checked_(false)
{
	setFixedSize(20, 20);
	setChecked(value);
}

void BoolBox::setChecked(bool value)
{
	if (value != checked_) {
		checked_ = value;
		Q_EMIT valueChanged(checked_);
		update();
	}
}
QVariant BoolBox::getValue()
{
	return checked_;
}

void BoolBox::setValue(QVariant var)
{
	setChecked(var.toBool());
}

void BoolBox::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	if (hovered_) {
		painter.setBrush(QColor(185, 185, 185));
	}
	else
		painter.setBrush(QColor(150, 150, 150));
	painter.drawRoundedRect(rect().adjusted(2, 2, -2, -2), 2, 2);

	if (checked_) {
		QPen pen(QColor(0, 0, 0));
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);
		pen.setWidth(2);
		painter.setPen(pen);
		QPainterPath m_path;
		m_path.moveTo(rect().center().x() - 9 + 6, 10);
		m_path.lineTo(rect().center().x() - 9 + 8, 14);
		m_path.lineTo(rect().center().x() - 9 + 14, 6);
		painter.drawPath(m_path);
	}
}

void BoolBox::mouseReleaseEvent(QMouseEvent* event)
{
	Button::mouseReleaseEvent(event);
	if (event->button() == Qt::LeftButton) {
		setChecked(!checked_);
	}
}