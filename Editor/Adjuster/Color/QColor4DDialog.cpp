#include "QColor4DDialog.hpp"
#include "QBoxLayout"

QColor4DDialog::QColor4DDialog()
	: wheel(new QWidgetEx::ColorWheel)
	, preview(new QWidgetEx::ColorPreview)
	, sliderRed(new QGradientSlider("R"))
	, sliderGreen(new QGradientSlider("G"))
	, sliderBlue(new QGradientSlider("B"))
	, sliderAlpha(new QGradientSlider("A"))
	, sliderHue(new QGradientSlider("H", 0.0f, 0.0f, 359.0f, true))
	, sliderSaturation(new QGradientSlider("S", 0.0f, 0.0f, 1.0f, true))
	, sliderValue(new QGradientSlider("V"))
	, btOk(new QPushButton("OK"))
	, btCancel(new QPushButton("Cancel"))
	, leHex(new QWidgetEx::ColorLineEdit())
{
	createUI();
	connectUI();
}

void QColor4DDialog::setColor(QColor4D color) {
	pre = color;
	preview->setComparisonColor(color);
	setCurrentColorInternal(color);
}

int QColor4DDialog::exec(QColor4D color)
{
	pre = color;
	preview->setComparisonColor(color);
	setCurrentColorInternal(color);
	return QDialog::exec();
}

void QColor4DDialog::createUI()
{
	QVBoxLayout* v = new QVBoxLayout(this);
	QHBoxLayout* h = new QHBoxLayout();
	h->addWidget(wheel, 5);
	QVBoxLayout* tool = new QVBoxLayout;
	tool->addWidget(preview);
	tool->addWidget(leHex);
	h->addLayout(tool, 0);

	v->addLayout(h, 1);
	QHBoxLayout* sliderPanle = new QHBoxLayout();
	sliderPanle->setAlignment(Qt::AlignBottom);
	QVBoxLayout* RGBA = new QVBoxLayout;
	QVBoxLayout* HSV = new QVBoxLayout;

	RGBA->setAlignment(Qt::AlignTop);
	RGBA->addWidget(sliderRed);
	RGBA->addWidget(sliderGreen);
	RGBA->addWidget(sliderBlue);
	RGBA->addWidget(sliderAlpha);

	HSV->setAlignment(Qt::AlignTop);
	HSV->addWidget(sliderHue);
	HSV->addWidget(sliderSaturation);
	HSV->addWidget(sliderValue);
	QHBoxLayout* buttonPanel = new QHBoxLayout();
	buttonPanel->setAlignment(Qt::AlignRight);
	buttonPanel->addWidget(btOk);
	buttonPanel->addWidget(btCancel);
	HSV->addLayout(buttonPanel);

	sliderPanle->addLayout(RGBA);
	sliderPanle->addLayout(HSV);
	v->addLayout(sliderPanle, 2);

	leHex->setFixedHeight(25);
	preview->setFixedHeight(40);
	preview->setDisplayMode(QWidgetEx::ColorPreview::SplitColor);
	wheel->setMinimumSize(300, 300);
	btOk->setFocusPolicy(Qt::NoFocus);
	btCancel->setFocusPolicy(Qt::NoFocus);
}

void QColor4DDialog::connectUI()
{
	connect(wheel, &QWidgetEx::ColorWheel::colorChanged, this, [this](QColor color) {
		setCurrentColorInternal(color);
	});
	connect(leHex, &QWidgetEx::ColorLineEdit::colorChanged, this, [this](QColor color) {
		setCurrentColorInternal(color);
	});

	connect(sliderRed, &QGradientSlider::valueChanged, this, [this](QVariant var) {
		QColor4D next = current;
		next.setRedF(var.toFloat());
		setCurrentColorInternal(next);
	});
	connect(sliderGreen, &QGradientSlider::valueChanged, this, [this](QVariant var) {
		QColor4D next = current;
		next.setGreenF(var.toFloat());
		setCurrentColorInternal(next);
	});
	connect(sliderBlue, &QGradientSlider::valueChanged, this, [this](QVariant var) {
		QColor4D next = current;
		next.setBlueF(var.toFloat());
		setCurrentColorInternal(next);
	});
	connect(sliderAlpha, &QGradientSlider::valueChanged, this, [this](QVariant var) {
		QColor4D next = current;
		next.setAlphaF(var.toFloat());
		setCurrentColorInternal(next);
	});

	connect(sliderHue, &QGradientSlider::valueChanged, this, [this](QVariant var) {
		QVector4D hsv = current.toHSVA();
		float v = var.toFloat();
		hsv.setX(var.toFloat());
		QColor4D next;
		next.fromHSVA(hsv);
		setCurrentColorInternal(next);
	});
	connect(sliderSaturation, &QGradientSlider::valueChanged, this, [this](QVariant var) {
		QVector4D hsv = current.toHSVA();
		hsv.setY(var.toFloat());
		QColor4D next;
		next.fromHSVA(hsv);
		setCurrentColorInternal(next);
	});
	connect(sliderValue, &QGradientSlider::valueChanged, this, [this](QVariant var) {
		QVector4D hsv = current.toHSVA();
		hsv.setZ(var.toFloat());
		QColor4D next;
		next.fromHSVA(hsv);
		setCurrentColorInternal(next);
	});

	connect(btOk, &QPushButton::clicked, this, [this]() {
		close();
	});
	connect(btCancel, &QPushButton::clicked, this, [this]() {
		setCurrentColorInternal(pre);
		close();
	});
}

void QColor4DDialog::setCurrentColorInternal(QColor4D color)
{
	if (current == color)
		return;
	current = color;

	wheel->blockSignals(true);
	preview->blockSignals(true);
	sliderRed->blockSignals(true);
	sliderGreen->blockSignals(true);
	sliderBlue->blockSignals(true);
	sliderAlpha->blockSignals(true);
	sliderHue->blockSignals(true);
	sliderSaturation->blockSignals(true);
	sliderValue->blockSignals(true);
	leHex->blockSignals(true);

	wheel->setColor(color);
	preview->setColor(color);
	leHex->setColor(color);
	sliderRed->setNumber(color.redF());
	sliderGreen->setNumber(color.greenF());
	sliderBlue->setNumber(color.blueF());
	sliderAlpha->setNumber(color.alphaF());
	sliderHue->setNumber(color.hsvHueF());
	sliderSaturation->setNumber(color.hsvSaturationF());
	sliderValue->setNumber(color.valueF());

	wheel->blockSignals(false);
	preview->blockSignals(false);
	sliderRed->blockSignals(false);
	sliderGreen->blockSignals(false);
	sliderBlue->blockSignals(false);
	sliderAlpha->blockSignals(false);
	sliderHue->blockSignals(false);
	sliderSaturation->blockSignals(false);
	sliderValue->blockSignals(false);
	leHex->blockSignals(false);

	Q_EMIT colorChanged(current);
}