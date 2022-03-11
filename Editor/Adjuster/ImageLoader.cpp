#include "ImageLoader.h"
#include <QFileDialog>
#include "Effect/QNeumorphism.h"
#include "QPainter"
#include "QBrush"

ImageLoader::ImageLoader(QImage value, QWidget* parent /*= nullptr*/)
{
	setFixedSize(60, 60);
	setValue(value);
	setGraphicsEffect(new QNeumorphism);
	connect(this, &Button::clicked, this, &ImageLoader::loadFile);
}

QVariant ImageLoader::getValue()
{
	return data_;
}

void ImageLoader::setValue(QVariant var)
{
	data_ = var.value<QImage>();
	if (data_.isNull())
		return;
	double wh = data_.width() / (double)data_.height();
	QSize cacheSize;
	if (wh < 1) {
		cacheSize.setWidth(60 * wh);
		cacheSize.setHeight(60);
	}
	else {
		cacheSize.setWidth(60);
		cacheSize.setHeight(60 / wh);
	}
	cache_ = QPixmap::fromImage(data_.scaled(cacheSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	Q_EMIT valueChanged(data_);
}

void ImageLoader::loadFile()
{
	QString filePath = QFileDialog::getOpenFileName(nullptr, "Load File");
	if (!QFile::exists(filePath))
		return;
	setValue(QVariant::fromValue(QImage(filePath)));
}

void ImageLoader::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	if (hovered_) {
		painter.setBrush(QColor(150, 150, 150));
	}
	else
		painter.setBrush(QColor(185, 185, 185));
	painter.drawRoundedRect(rect(), 2, 2);
	if (cache_.isNull()) {
		painter.drawText(rect(), Qt::AlignCenter, "Load Image");
	}
	else {
		QRect showRect(0, 0, cache_.width(), cache_.height());
		showRect.moveCenter(rect().center());
		painter.drawPixmap(showRect, cache_);
	}
}