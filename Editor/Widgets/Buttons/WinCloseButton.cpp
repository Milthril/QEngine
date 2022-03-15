#include "WinCloseButton.h"
#include <QPainter>
#include <QStyleOption>

WinCloseButton::WinCloseButton()
{
	hoverColor = Qt::red;
}

void WinCloseButton::paintEvent(QPaintEvent* e)
{
	HoverButton::paintEvent(e);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen;
	QStyleOption options;
	options.initFrom(this);
	pen.setBrush(options.palette.color(QPalette::Text));
	pen.setCapStyle(Qt::RoundCap);
	pen.setWidth(2);
	QRect re = rect().adjusted(4, 4, -4, -4);
	painter.setPen(pen);
	painter.drawLine(QPoint(re.left(), re.bottom()), QPoint(re.right(), re.top()));
	painter.drawLine(QPoint(re.left(), re.top()), QPoint(re.right(), re.bottom()));
} 