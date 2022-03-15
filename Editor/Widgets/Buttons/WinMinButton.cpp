#include "WinMinButton.h"
#include "QPainter"
#include <QStyleOption>

WinMinButton::WinMinButton()
{
}

void WinMinButton::paintEvent(QPaintEvent* e) {
	HoverButton::paintEvent(e);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen;
	QStyleOption options;
	options.initFrom(this);
	pen.setBrush(options.palette.color(QPalette::Text));
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setWidth(2);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(pen);
	painter.drawRect(rect().adjusted(5, 10, -5, -10));
}