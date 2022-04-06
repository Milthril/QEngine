#include "ImageButton.h"
#include <QPainter>
#include "QDebug"

ImageButton::ImageButton(QString path)
	:mIcon(path){
}

void ImageButton::paintEvent(QPaintEvent* e)
{
	HoverButton::paintEvent(e);
	QPainter painter(this);
	mIcon.getIcon().paint(&painter, rect());
}
