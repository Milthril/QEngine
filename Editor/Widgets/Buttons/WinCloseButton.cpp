#include "WinCloseButton.h"
#include <QPainter>
#include <QStyleOption>

WinCloseButton::WinCloseButton()
{
}

void WinCloseButton::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen;
	QStyleOption options;
	options.initFrom(this);
	if (hovered) {
		pen.setBrush(QColor(221, 83, 71));
		pen.setWidth(3);
	}
	else {
		pen.setBrush(options.palette.color(QPalette::Text));
		pen.setWidth(1);
	}
	pen.setCapStyle(Qt::RoundCap);
	QRect re = rect().adjusted(4, 4, -4, -4);
	painter.setPen(pen);
	painter.drawLine(QPoint(re.left(), re.bottom()), QPoint(re.right(), re.top()));
	painter.drawLine(QPoint(re.left(), re.top()), QPoint(re.right(), re.bottom()));
} 