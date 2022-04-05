#include <QPainter>
#include "WinMaxButton.h"
#include <QStyleOption>

WinMaxButton::WinMaxButton()
{
}

void WinMaxButton::paintEvent(QPaintEvent* e) {
	HoverButton::paintEvent(e);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen;
	QStyleOption options;
	options.initFrom(this);
	pen.setBrush(options.palette.color(QPalette::Text));
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setWidth(1);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(pen);
	painter.drawRect(rect().adjusted(5, 5, -5, -5));
}