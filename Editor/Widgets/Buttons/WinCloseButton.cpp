#include "WinCloseButton.h"
#include <QPainter>
#include <QStyleOption>

WinCloseButton::WinCloseButton()
	:mIcon(":/Resources/Icons/Shut down-2.png")
{
	setHoverColor(QColor(250, 50, 50));
}

void WinCloseButton::paintEvent(QPaintEvent* e)
{
	HoverButton::paintEvent(e);
	QPainter painter(this);
	mIcon.getIcon().paint(&painter, rect().adjusted(2,2,-2,-2));
} 