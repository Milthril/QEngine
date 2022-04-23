#include "QColor4DButton.hpp"
#include "Toolkit/QWidgetShadowMaker.h"
#include "ColorDialog.hpp"
#include <QPainter>
#include "QColor4DDialog.hpp"
#include "kddockwidgets/DockWidget.h"

QColor4DButton::QColor4DButton(QColor4D color)
	: color_(color){
	setMinimumWidth(100);
	setFixedHeight(20);
	setColor(color);
	QObject::connect(this, &Button::clicked, this, [&]() {
		auto dockWidget = new KDDockWidgets::DockWidget("World", KDDockWidgets::DockWidget::Option_None);
		auto dialog = new QColor4DDialog;
		dialog->setColor(color_);
		connect(dialog, &QColor4DDialog::colorChanged, this, [this](QColor4D color) {
			setColor(color);
			Q_EMIT valueChanged(QVariant::fromValue(color));
		});
		dockWidget->setWidget(dialog);
		dockWidget->setAttribute(Qt::WA_DeleteOnClose);
		dockWidget->show();
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