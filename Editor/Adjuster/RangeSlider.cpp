#include "RangeSlider.h"
#include "QFocusLineEdit.h"
#include "Toolkit/QNeumorphism.h"
#include <QValidator>
#include <QBoxLayout>
#include <QPainter>
#include "Component\CustomSlider.h"

RangeSlider::RangeSlider(QRange range, QWidget* parent /*= nullptr*/)
	: range_(range)
	, lowerEditer_(new QFocusLineEdit)
	, slider_(new CustomSlider(Qt::Horizontal))
	, upperEditer_(new QFocusLineEdit)
{
	setFixedHeight(20);
	setGraphicsEffect(new QNeumorphism);

	lowerEditer_->setFixedHeight(height());
	lowerEditer_->setFrame(QFrame::NoFrame);
	lowerEditer_->setAlignment(Qt::AlignCenter);
	lowerEditer_->setValidator(new QIntValidator);
	lowerEditer_->setStyleSheet("background-color:transparent;");
	lowerEditer_->setFixedWidth(50);

	connect(slider_, &CustomSlider::lowerValueChanged, this, [this](int var) {
		lowerEditer_->setText(QString::number(var));
		range_.setLower(var);
		Q_EMIT valueChanged(QVariant::fromValue(range_));
	});

	connect(slider_, &CustomSlider::upperValueChanged, this, [this](int var) {
		upperEditer_->setText(QString::number(var));
		range_.setUpper(var);
		Q_EMIT valueChanged(QVariant::fromValue(range_));
	});

	slider_->setMinimumWidth(80);
	slider_->SetMaximum(range_.max());
	slider_->SetMinimum(range_.min());
	slider_->SetLowerValue(range_.getLower());
	slider_->SetUpperValue(range_.getUpper());

	connect(lowerEditer_, &QFocusLineEdit::textChanged, this, [this](QString text) {
		int var = text.toInt();
		if (var != range_.getLower()) {
			slider_->SetLowerValue(var);
			if (slider_->GetLowerValue() != var) {
				lowerEditer_->setText(QString::number(slider_->GetLowerValue()));
			}
		}
	});

	connect(upperEditer_, &QFocusLineEdit::textChanged, this, [this](QString text) {
		int var = text.toInt();
		if (var != range_.getUpper()) {
			slider_->SetUpperValue(var);
			if (slider_->GetUpperValue() != var) {
				upperEditer_->setText(QString::number(slider_->GetUpperValue()));
			}
		}
	});

	upperEditer_->setFixedHeight(height());
	upperEditer_->setFrame(QFrame::NoFrame);
	upperEditer_->setAlignment(Qt::AlignCenter);
	upperEditer_->setValidator(new QDoubleValidator);
	upperEditer_->setStyleSheet("background-color:transparent;");
	upperEditer_->setFixedWidth(50);

	QHBoxLayout* h = new QHBoxLayout(this);
	h->setContentsMargins(0, 0, 0, 0);
	h->setSpacing(0);
	h->addWidget(lowerEditer_);
	h->addWidget(slider_);
	h->addWidget(upperEditer_);
	slider_->raise();
	setValue(QVariant::fromValue(range));
}

RangeSlider::~RangeSlider()
{
}

QVariant RangeSlider::getValue()
{
	return QVariant::fromValue<>(range_);
}

void RangeSlider::setValue(QVariant var)
{
	QRange range = var.value<QRange>();
	if (range_ == range)
		return;
	range_ = range;
	slider_->SetLowerValue(range.getLower());
	slider_->SetUpperValue(range.getUpper());
}

void RangeSlider::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(200, 200, 200));
	painter.drawRoundedRect(rect(), 2, 2);
	QWidget::paintEvent(event);
}