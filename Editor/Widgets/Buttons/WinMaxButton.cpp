#include <QPainter>
#include "WinMaxButton.h"

WinMaxButton::WinMaxButton()
{

}

void WinMaxButton::paintEvent(QPaintEvent* e) {
	HoverButton::paintEvent(e);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen;
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(pen);
	painter.drawRect(rect().adjusted(10, 10, -10, -10));
}