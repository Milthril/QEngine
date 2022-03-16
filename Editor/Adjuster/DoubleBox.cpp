#include "DoubleBox.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QLabel>
#include <QDoubleValidator>
#include <QPainter>
#include "QFocusLineEdit.h"
#include "Effect/QNeumorphism.h"

DoubleBox::DoubleBox(double value /*= 0*/, QString name, QWidget* parent /*= nullptr*/)
	: Adjuster(parent)
	, nameLabel_(new QLabel(name))
	, numberEditer_(new QFocusLineEdit)
	, arrowLabel_(new QLabel)
{
	setFixedHeight(20);
	setGraphicsEffect(new QNeumorphism);
	QHBoxLayout* h = new QHBoxLayout(this);
	h->setContentsMargins(0, 0, 0, 0);
	h->addWidget(nameLabel_);
	h->addWidget(numberEditer_);
	h->addWidget(arrowLabel_);
	nameLabel_->setContentsMargins(2, 0, 0, 2);
	nameLabel_->setAlignment(Qt::AlignCenter);
	nameLabel_->setFixedHeight(height());
	if (nameLabel_->text().isEmpty())
		nameLabel_->setVisible(false);
	arrowLabel_->setFixedSize(height(), height());
	arrowLabel_->setPixmap(QPixmap(":/Resources/Icons/box_arrow").scaled(arrowLabel_->size(), Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	arrowLabel_->setCursor(Qt::CursorShape::SizeHorCursor);
	numberEditer_->setFixedHeight(height());
	numberEditer_->setFrame(QFrame::NoFrame);
	numberEditer_->setValidator(new QDoubleValidator);
	numberEditer_->setAlignment(Qt::AlignLeft);
	numberEditer_->setMinimumWidth(50);
	setNumber(value);
	setEditEnabled(false);
	connect(numberEditer_, &QFocusLineEdit::loseFocus, this, [this]() {
		setEditEnabled(false);
	});
	connect(numberEditer_, &QLineEdit::textChanged, this, [this](QString) {
		Q_EMIT valueChanged(number());
	});
}

DoubleBox::~DoubleBox()
{
}

double DoubleBox::number()
{
	return numberEditer_->text().toDouble();
}

void DoubleBox::setNumber(double num) {
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

void DoubleBox::setEditEnabled(bool enable) {
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

bool DoubleBox::getEditEnabled()
{
	return numberEditer_->focusPolicy() == Qt::FocusPolicy::StrongFocus;
}

QVariant DoubleBox::getValue() {
	return number();
}

void DoubleBox::setValue(QVariant var)
{
	setNumber(var.toDouble());
}

void DoubleBox::moveBox(QPointF offset)
{
	double adjuster = number() + offset.x() * qMax(qAbs(number() / 200), 0.01);
	adjuster = int(adjuster * 100) / 100.0;
	setNumber(adjuster);
}

void DoubleBox::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		clickPosition_ = event->pos();
		if (getEditEnabled() && arrowLabel_->geometry().contains(event->pos())) {
			setEditEnabled(false);
		}
	}
}

void DoubleBox::mouseReleaseEvent(QMouseEvent* event)
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

void DoubleBox::mouseMoveEvent(QMouseEvent* event)
{
	if (!getEditEnabled()) {
		setCursor(Qt::BlankCursor);
		arrowLabel_->setCursor(Qt::CursorShape::BlankCursor);
		QPointF offset = event->position() - clickPosition_;
		moveBox(offset);
		QCursor::setPos(mapToGlobal(clickPosition_.toPoint()));
	}
}

void DoubleBox::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(200, 200, 200));
	painter.drawRoundedRect(rect(), 2, 2);
	QWidget::paintEvent(event);
}