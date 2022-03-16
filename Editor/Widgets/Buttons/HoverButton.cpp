#include "HoverButton.h"
#include <QPainter>

void HoverButton::enterEvent(QEnterEvent* event)
{
	hovered = true;
	update();
}

void HoverButton::leaveEvent(QEvent* event)
{
	hovered = false;
	update();
}

void HoverButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	if (this->isChecked()) {
		painter.fillRect(rect(), checkColor);
	}
	if (hovered) {
		painter.fillRect(rect(), hoverColor);
	}
}