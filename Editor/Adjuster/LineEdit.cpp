#include "LineEdit.h"
#include "QFocusLineEdit.h"

#include <QHBoxLayout>
#include "Effect/QNeumorphism.h"
#include "QPainter"

LineEdit::LineEdit(QString str)
	:lineEdit_(new QFocusLineEdit())
{
	setGraphicsEffect(new QNeumorphism);
	QHBoxLayout* h = new QHBoxLayout(this);
	h->setContentsMargins(0, 0, 0, 0);
	h->addWidget(lineEdit_);
	lineEdit_->setFixedHeight(20);
	lineEdit_->setText(str);
	lineEdit_->setFrame(QFrame::NoFrame);
	lineEdit_->setStyleSheet("background-color:transparent");
	connect(lineEdit_, &QLineEdit::textChanged, this, [&](QString str) {
		Q_EMIT valueChanged(str);
	});
}

QVariant LineEdit::getValue()
{
	return lineEdit_->text();
}

void LineEdit::setValue(QVariant var)
{
	lineEdit_->setText(var.toString());
}

void LineEdit::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(200, 200, 200));
	painter.drawRoundedRect(rect(), 2, 2);
	QWidget::paintEvent(event);
}