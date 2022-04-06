#include "QColor4DButton.hpp"
#include "Toolkit/QWidgetShadowMaker.h"
#include "ColorDialog.hpp"
#include <QPainter>
#include "QColor4DDialog.hpp"

QColor4DButton::QColor4DButton(QColor4D color)
	: color_(color)
{
	setMinimumWidth(100);
	setFixedHeight(20);
	setColor(color);
	QObject::connect(this, &Button::clicked, this, [&]() {
		QColor4DDialog dialog;
		QObject::connect(&dialog, &QColor4DDialog::colorChanged, this, [&](const QColor4D& color) {
			setColor(color);
			Q_EMIT valueChanged(QVariant::fromValue(color_));
		});
		dialog.exec(color_);
	});
}

void QColor4DButton::setColor(QColor4D color)
{
	color_ = color;
	update();
}

QVariant QColor4DButton::getValue()
{
	return color_;
}

void QColor4DButton::setValue(QVariant var)
{
	setColor(var.value<QColor4D>());
}

void QColor4DButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(color_.toQColor());
	painter.drawRoundedRect(rect(), 2, 2);
}