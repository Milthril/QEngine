#include "WinMinButton.h"
#include "QPainter"

WinMinButton::WinMinButton()
{
}

void WinMinButton::paintEvent(QPaintEvent* e) {
	HoverButton::paintEvent(e);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen;
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(pen);
	painter.drawRect(rect().adjusted(10, 15, -10, -15));
}