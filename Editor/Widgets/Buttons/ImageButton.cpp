#include "ImageButton.h"
#include <QPainter>
#include "QDebug"

ImageButton::ImageButton(QString path) :ImageButton(QPixmap(path)) {}

ImageButton::ImageButton(QPixmap image)
{
	pixmap_ = image;
}

void ImageButton::paintEvent(QPaintEvent* e)
{
	HoverButton::paintEvent(e);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawPixmap(rect(), showMap_);
}

void ImageButton::resizeEvent(QResizeEvent* event)
{
	showMap_ = pixmap_.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}