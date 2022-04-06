#include <QPainter>
#include "WinMaxButton.h"
#include <QStyleOption>

WinMaxButton::WinMaxButton()
	:mFullScreen(":/Resources/Icons/Maximize-3.png")
	,mWindow(":/Resources/Icons/Maximize-1.png")
{
}

void WinMaxButton::paintEvent(QPaintEvent* e) {
	HoverButton::paintEvent(e);
	QPainter painter(this);
	updateTopLevelWidget();
	if(mTopLevelWidget && mTopLevelWidget->isMaximized())
		mFullScreen.getIcon().paint(&painter, rect().adjusted(2, 2, -2, -2));
	else 
		mWindow.getIcon().paint(&painter, rect().adjusted(2, 2, -2, -2));
}

void WinMaxButton::updateTopLevelWidget() {
	if (mTopLevelWidget == nullptr) {
		QWidget* parent = parentWidget();
		if (parent == nullptr)
			return;
		while (parent->parentWidget() != nullptr) {
			parent = parent->parentWidget();
		}
		mTopLevelWidget = parent;
	}
}
