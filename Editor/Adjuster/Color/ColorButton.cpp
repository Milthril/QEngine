#include "ColorButton.hpp"
#include "Toolkit/QNeumorphism.h"
#include "ColorDialog.hpp"
#include <QPainter>

ColorButton::ColorButton(QColor color)
	: color_(color)
{
	setMinimumWidth(100);
	setFixedHeight(20);

	setColor(color);
	QObject::connect(this, &Button::clicked, this, [&]() {
		QWidgetEx::ColorDialog dialog;
		dialog.setColor(color_);
		QObject::connect(&dialog, &QWidgetEx::ColorDialog::colorChanged, this, [&](const QColor& color) {
			setColor(color);
			Q_EMIT valueChanged(QVariant::fromValue(color_));
		});
		dialog.exec();
	});
}

void ColorButton::setColor(QColor color)
{
	color_ = color;
	update();
}

QVariant ColorButton::getValue()
{
	return color_;
}

void ColorButton::setValue(QVariant var)
{
	setColor(var.value<QColor>());
}

void ColorButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(color_);
	painter.drawRoundedRect(rect(), 2, 2);
}