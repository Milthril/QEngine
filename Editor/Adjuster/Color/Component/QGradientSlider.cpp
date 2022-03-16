#include "QGradientSlider.hpp"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QLabel>
#include <QDoubleValidator>
#include <QPainter>
#include "Effect/QNeumorphism.h"

QGradientSlider::QGradientSlider(QString name /*= ""*/, float var /*= 0.0f*/, float min /*= 0.0f*/, float max /*= 1.0f*/, bool limited /*=false*/, QWidget* parent /*= nullptr*/)
	: Adjuster(parent)
	, nameLabel_(new QLabel(name))
	, numberEditer_(new QFocusLineEdit)
	, arrowLabel_(new QLabel)
	, var_(var)
	, min_(min)
	, max_(max)
	, limited_(limited)
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
	arrowLabel_->setPixmap(QPixmap(":/Resources/Icons/box_arrow").scaled(arrowLabel_->size(), Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	arrowLabel_->setCursor(Qt::CursorShape::SizeHorCursor);
	numberEditer_->setFixedHeight(height());
	numberEditer_->setFrame(QFrame::NoFrame);
	numberEditer_->setValidator(new QDoubleValidator);
	numberEditer_->setStyleSheet("background-color:transparent;");
	//numberEditer_->setAlignment(Qt::AlignRight);
	setNumber(var);
	setEditEnabled(false);
	connect(numberEditer_, &QFocusLineEdit::loseFocus, this, [this]() {
		setEditEnabled(false);
	});
	connect(numberEditer_, &QLineEdit::returnPressed, this, [this]() {
		float num = this->number();
		setNumber(num);
		Q_EMIT valueChanged(var_);
	});
}

QGradientSlider::~QGradientSlider()
{
}

float QGradientSlider::number()
{
	return numberEditer_->text().toFloat();
}

void QGradientSlider::setNumber(float num) {
	if (limited_)
		num = qBound(min_, num, max_);
	var_ = num;
	int precision = qBound(0, 12 - (int)log10(qAbs(num)), 6);
	QString text = QString::number(num, 'F', precision);
	int index = text.size() - 1;
	while (index > 0 && text[index] == '0') {
		index--;
	}
	if (text[index] == '.')
		index++;
	numberEditer_->setText(text.mid(0, index + 1));
	update();
}

void QGradientSlider::setEditEnabled(bool enable) {
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

bool QGradientSlider::getEditEnabled()
{
	return numberEditer_->focusPolicy() == Qt::FocusPolicy::StrongFocus;
}

QVariant QGradientSlider::getValue()
{
	return var_;
}

void QGradientSlider::setValue(QVariant var)
{
	setNumber(var.toFloat());
}

void QGradientSlider::moveBox(QPointF offset)
{
	float var = qBound(min_, number() + offset.x() * (max_ - min_) / 1000, max_);
	setNumber(var);
}

void QGradientSlider::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		clickPosition_ = event->pos();
		if (getEditEnabled() && arrowLabel_->geometry().contains(event->pos())) {
			setEditEnabled(false);
		}
	}
}

void QGradientSlider::mouseReleaseEvent(QMouseEvent* event)
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

void QGradientSlider::mouseMoveEvent(QMouseEvent* event)
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

void QGradientSlider::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(mBrush);
	painter.drawRoundedRect(rect(), 2, 2);

	QRect slider = rect();
	slider.setRight(rect().width() * ((var_ - min_) / (max_ - min_)));
	painter.setBrush(QColor(140, 140, 140));
	painter.drawRoundedRect(slider, 2, 2);
	QWidget::paintEvent(event);
}