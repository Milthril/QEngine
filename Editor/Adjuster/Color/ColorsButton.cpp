#include "ColorsButton.hpp"
#include "Toolkit/QNeumorphism.h"
#include "ColorsDialog.hpp"
#include <QPainter>
#include <QHBoxLayout>

ColorsButton::ColorsButton(QColors colors)
	: colors_(colors)
{
	setMinimumWidth(100);
	setFixedHeight(20);

	setColors(colors);
	QObject::connect(this, &Button::clicked, this, [&]() {
		QWidgetEx::ColorsDialog dialog;
		dialog.setColors(colors_.getStops());
		QObject::connect(&dialog, &QWidgetEx::ColorsDialog::colorsChanged, this, [&](const QGradientStops& stops) {
			setColors(QColors(stops));
			Q_EMIT valueChanged(QVariant::fromValue(colors_));
		});
		dialog.exec();
	});
}

void ColorsButton::setColors(QColors colors)
{
	colors_ = colors;
	update();
}

QVariant ColorsButton::getValue()
{
	return QVariant::fromValue(colors_);
}

void ColorsButton::setValue(QVariant var)
{
	setColors(var.value<QColors>());
}

void ColorsButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QLinearGradient linearGradient;
	linearGradient.setStops(colors_.getStops());
	linearGradient.setStart({ 0,0 });
	linearGradient.setFinalStop({ (qreal)width(),0.0 });

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(linearGradient);
	painter.drawRoundedRect(rect(), 2, 2);
}