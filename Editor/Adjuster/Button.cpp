#include "Button.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include "Effect/QNeumorphism.h"

Button::Button(QWidget* parent)
	: Adjuster(parent)
	, hovered_(false)
{
	this->setGraphicsEffect(new QNeumorphism);
}

void Button::enterEvent(QEnterEvent* event)
{
	hovered_ = true;
	update();
}

void Button::leaveEvent(QEvent* event)
{
	hovered_ = false;
	update();
}

void Button::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		clickPosition_ = event->pos();
	}
}

void Button::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		if (clickPosition_ == event->pos()) {
			Q_EMIT clicked();
		}
	}
}

void Button::paintEvent(QPaintEvent* event)
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
}