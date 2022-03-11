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
	if (hovered) {
		QPainter painter(this);
		painter.fillRect(rect(), hoverColor);
	}
}