#include "WinMinButton.h"
#include "QPainter"
#include <QStyleOption>

WinMinButton::WinMinButton()
	:mIcon(":/Resources/Icons/Minimize-2.png") {
}

void WinMinButton::paintEvent(QPaintEvent* e) {
	HoverButton::paintEvent(e);
	QPainter painter(this);
	mIcon.getIcon().paint(&painter, rect().adjusted(2, 2, -2, -2));
}	