#include "WinCloseButton.h"
#include <QPainter>

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
	pen.setCapStyle(Qt::RoundCap);
	pen.setWidth(1);
	QRect re = rect().adjusted(10, 10, -10, -10);
	painter.setPen(pen);
	painter.drawLine(re.bottomLeft(), re.topRight());
	painter.drawLine(re.topLeft(), re.bottomRight());
}