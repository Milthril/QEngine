#include "DoubleSlider.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QLabel>
#include <QDoubleValidator>
#include <QPainter>
#include "QFocusLineEdit.h"
#include "Toolkit/QNeumorphism.h"

DoubleSlider::DoubleSlider(QBoundedDouble number  /*= 0*/, QString name, QWidget* parent /*= nullptr*/)
	: Adjuster(parent)
	, nameLabel_(new QLabel(name))
	, numberEditer_(new QFocusLineEdit)
	, arrowLabel_(new QLabel)
	, value_(number)
{
	setFixedHeight(20);
	setGraphicsEffect(new QNeumorphism);
	QHBoxLayout* h = new QHBoxLayout(this);
	h->setContentsMargins(5, 0, 0, 0);
	h->setSpacing(0);
	h->addWidget(nameLabel_);
	h->addWidget(numberEditer_);
	h->addWidget(arrowLabel_);
	if (nameLabel_->text().isEmpty())
		nameLabel_->setVisible(false);
	nameLabel_->setContentsMargins(0, 0, 0, 2);
	nameLabel_->setAlignment(Qt::AlignCenter);
	nameLabel_->setFixedHeight(height());
	arrowLabel_->setFixedSize(height(), height());
	arrowLabel_->setPixmap(QPixmap(":/box_arrow").scaled(arrowLabel_->size(), Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	arrowLabel_->setCursor(Qt::CursorShape::SizeHorCursor);
	numberEditer_->setFixedHeight(height());
	numberEditer_->setFrame(QFrame::NoFrame);
	numberEditer_->setValidator(new QDoubleValidator);
	//numberEditer_->setAlignment(Qt::AlignRight);
	setNumber(value_.number());
	setEditEnabled(false);
	connect(numberEditer_, &QFocusLineEdit::loseFocus, this, [this]() {
		setEditEnabled(false);
	});
	connect(numberEditer_, &QLineEdit::textChanged, this, [this](QString) {
		double num = this->number();
		if (num == value_.number()) {
			return;
		}
		if (value_.setNumber(this->number())) {
			Q_EMIT valueChanged(QVariant::fromValue(value_));
		}
		else {
			setNumber(value_.number());
		}
	});
}

DoubleSlider::~DoubleSlider()
{
}

double DoubleSlider::number()
{
	return numberEditer_->text().toDouble();
}

void DoubleSlider::setNumber(double num) {
	int precision = qBound(0, 12 - (int)log10(qAbs(num)), 6);
	QString text = QString::number(num, 'F', precision);
	int index = text.size() - 1;
	while (index > 0 && text[index] == '0') {
		index--;
	}
	if (text[index] == '.')
		index++;
	numberEditer_->setText(text.mid(0, index + 1));
}

void DoubleSlider::setEditEnabled(bool enable) {
	if (enable) {
		numberEditer_->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
		numberEditer_->setAttribute(Qt::WA_TransparentForMouseEvents, false);
		setCursor(Qt::CursorShape::IBeamCursor);
		numberEditer_->activateWindow();
		numberEditer_->setFocus();
		numberEditer_->setReadOnly(false);
		numberEditer_->selectAll();
	}
	else {
		numberEditer_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
		numberEditer_->setAttribute(Qt::WA_TransparentForMouseEvents, true);
		setCursor(Qt::CursorShape::SizeHorCursor);
		numberEditer_->setReadOnly(true);
	}
}

bool DoubleSlider::getEditEnabled()
{
	return numberEditer_->focusPolicy() == Qt::FocusPolicy::StrongFocus;
}

QVariant DoubleSlider::getValue()
{
	return QVariant::fromValue(value_);
}

void DoubleSlider::setValue(QVariant var)
{
	setNumber(var.value<QBoundedDouble>().number());
}

void DoubleSlider::moveBox(QPointF offset)
{
	setNumber(number() + offset.x() * (value_.max() - value_.min()) / 2000);
}

void DoubleSlider::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		clickPosition_ = event->pos();
		if (getEditEnabled() && arrowLabel_->geometry().contains(event->pos())) {
			setEditEnabled(false);
		}
	}
}

void DoubleSlider::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		if (this->cursor() == Qt::BlankCursor) {
			setCursor(Qt::CursorShape::SizeHorCursor);
			arrowLabel_->setCursor(Qt::CursorShape::SizeHorCursor);
		}
		else if (clickPosition_ == event->pos() && !getEditEnabled() && this->cursor() != Qt::BlankCursor) {
			setEditEnabled(true);
		}
	}
}

void DoubleSlider::mouseMoveEvent(QMouseEvent* event)
{
	if (!getEditEnabled()) {
		setCursor(Qt::BlankCursor);
		arrowLabel_->setCursor(Qt::CursorShape::BlankCursor);
		QPointF offset = event->position() - clickPosition_;
		moveBox(offset);
		QCursor::setPos(mapToGlobal(clickPosition_.toPoint()));
		update();
	}
}

void DoubleSlider::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(mBrush);
	painter.drawRoundedRect(rect(), 2, 2);

	QRect slider = rect();
	slider.setRight(rect().width() * ((value_.number() - value_.min()) / (value_.max() - value_.min())));
	painter.setBrush(QColor(140, 140, 140));
	painter.drawRoundedRect(slider, 2, 2);
	QWidget::paintEvent(event);
}