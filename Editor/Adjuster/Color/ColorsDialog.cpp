#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPushButton>
#include <QScreen>
#include <QDebug>
#include "ColorsDialogUI.hpp"
#include "ColorsDialog.hpp"
#include "Component\ColorUtils.hpp"

namespace QWidgetEx {
class ColorsDialog::Private
{
public:
	Ui_ColorsDialog ui;
	ButtonMode button_mode;
	bool pick_from_screen;
	bool alpha_enabled;
	QColor color;

	Private() : pick_from_screen(false), alpha_enabled(true)
	{
	}

#ifdef Q_OS_ANDROID
	void screen_rotation()
	{
		auto scr = QApplication::primaryScreen();
		if (scr->size().height() > scr->size().width())
			ui.layout_main->setDirection(QBoxLayout::TopToBottom);
		else
			ui.layout_main->setDirection(QBoxLayout::LeftToRight);
	}
#endif
};

ColorsDialog::ColorsDialog(QWidget* parent, Qt::WindowFlags f) :
	QDialog(parent, f), p(new Private)
{
	p->ui.setupUi(this);
	setStyleSheet("QDialog{ background-color: rgb(255,255,255);}");
	setAcceptDrops(true);

#ifdef Q_OS_ANDROID
	connect(
		QGuiApplication::primaryScreen(),
		&QScreen::primaryOrientationChanged,
		this,
		[this] {p->screen_rotation(); }
	);
	p->ui.wheel->setWheelWidth(50);
	p->screen_rotation();
#endif

	connect(p->ui.wheel, &ColorWheel::colorSpaceChanged, this, &ColorsDialog::colorSpaceChanged);
	connect(p->ui.wheel, &ColorWheel::selectorShapeChanged, this, &ColorsDialog::wheelShapeChanged);
	connect(p->ui.wheel, &ColorWheel::rotatingSelectorChanged, this, &ColorsDialog::wheelRotatingChanged);
	connect(p->ui.gradient_bar, &GradientBar::currentItemChanged, this, &ColorsDialog::setColorInternal);
	connect(p->ui.gradient_bar, &GradientBar::currentItemPositionChanged, this, [this]() {
		Q_EMIT colorsChanged(colors());
		});
}

ColorsDialog::~ColorsDialog()
{
	delete p;
}

QSize ColorsDialog::sizeHint() const
{
	return QSize(400, 0);
}

QColor ColorsDialog::color() const
{
	QColor col = p->color;
	if (!p->alpha_enabled)
		col.setAlpha(255);
	return col;
}

void ColorsDialog::setCurrentColor(const QColor& c)
{
	p->ui.gradient_bar->setCurrentColor(c);
	p->ui.edit_hex->setModified(false);
	setColorInternal(c);
}

void ColorsDialog::showColor(const QColor& c)
{
	setCurrentColor(c);
	show();
}

void ColorsDialog::showColors(const QGradientStops& oldColors)
{
	setColors(oldColors);
	show();
}

void ColorsDialog::setAlphaEnabled(bool a)
{
	if (a != p->alpha_enabled)
	{
		p->alpha_enabled = a;

		p->ui.edit_hex->setShowAlpha(a);
		p->ui.line_alpha->setVisible(a);
		p->ui.label_alpha->setVisible(a);
		p->ui.slide_alpha->setVisible(a);
		p->ui.spin_alpha->setVisible(a);

		Q_EMIT alphaEnabledChanged(a);
	}
}

void ColorsDialog::setColors(QGradientStops colors)
{
	p->ui.gradient_bar->setColors(colors);
}

QGradientStops ColorsDialog::colors()
{
	return this->p->ui.gradient_bar->colors();
}

bool ColorsDialog::alphaEnabled() const
{
	return p->alpha_enabled;
}

void ColorsDialog::setColorInternal(const QColor& col)
{
	p->ui.wheel->setColor(col);

	p->color = col;

	bool blocked = signalsBlocked();
	blockSignals(true);
	Q_FOREACH(QWidget * w, findChildren<QWidget*>())
		w->blockSignals(true);

	p->ui.slide_red->setValue(col.red());
	p->ui.spin_red->setValue(p->ui.slide_red->value());
	p->ui.slide_red->setFirstColor(QColor(0, col.green(), col.blue()));
	p->ui.slide_red->setLastColor(QColor(255, col.green(), col.blue()));

	p->ui.slide_green->setValue(col.green());
	p->ui.spin_green->setValue(p->ui.slide_green->value());
	p->ui.slide_green->setFirstColor(QColor(col.red(), 0, col.blue()));
	p->ui.slide_green->setLastColor(QColor(col.red(), 255, col.blue()));

	p->ui.slide_blue->setValue(col.blue());
	p->ui.spin_blue->setValue(p->ui.slide_blue->value());
	p->ui.slide_blue->setFirstColor(QColor(col.red(), col.green(), 0));
	p->ui.slide_blue->setLastColor(QColor(col.red(), col.green(), 255));

	p->ui.slide_hue->setValue(qRound(p->ui.wheel->hue() * 360.0));
	p->ui.slide_hue->setColorSaturation(p->ui.wheel->saturation());
	p->ui.slide_hue->setColorValue(p->ui.wheel->value());
	p->ui.spin_hue->setValue(p->ui.slide_hue->value());

	p->ui.slide_saturation->setValue(qRound(p->ui.wheel->saturation() * 255.0));
	p->ui.spin_saturation->setValue(p->ui.slide_saturation->value());
	p->ui.slide_saturation->setFirstColor(QColor::fromHsvF(p->ui.wheel->hue(), 0, p->ui.wheel->value()));
	p->ui.slide_saturation->setLastColor(QColor::fromHsvF(p->ui.wheel->hue(), 1, p->ui.wheel->value()));

	p->ui.slide_value->setValue(qRound(p->ui.wheel->value() * 255.0));
	p->ui.spin_value->setValue(p->ui.slide_value->value());
	p->ui.slide_value->setFirstColor(QColor::fromHsvF(p->ui.wheel->hue(), p->ui.wheel->saturation(), 0));
	p->ui.slide_value->setLastColor(QColor::fromHsvF(p->ui.wheel->hue(), p->ui.wheel->saturation(), 1));

	QColor apha_color = col;
	apha_color.setAlpha(0);
	p->ui.slide_alpha->setFirstColor(apha_color);
	apha_color.setAlpha(255);
	p->ui.slide_alpha->setLastColor(apha_color);
	p->ui.spin_alpha->setValue(col.alpha());
	p->ui.slide_alpha->setValue(col.alpha());

	if (!p->ui.edit_hex->isModified())
		p->ui.edit_hex->setColor(col);

	p->ui.gradient_bar->setCurrentColor(col);

	blockSignals(blocked);
	Q_FOREACH(QWidget * w, findChildren<QWidget*>())
		w->blockSignals(false);

	Q_EMIT colorChanged(col);
	Q_EMIT colorsChanged(colors());
}

void ColorsDialog::set_hsv()
{
	if (!signalsBlocked())
	{
		QColor col = QColor::fromHsv(
			p->ui.slide_hue->value(),
			p->ui.slide_saturation->value(),
			p->ui.slide_value->value(),
			p->ui.slide_alpha->value()
		);
		p->ui.wheel->setColor(col);
		setColorInternal(col);
	}
}

void ColorsDialog::set_alpha()
{
	if (!signalsBlocked())
	{
		QColor col = p->color;
		col.setAlpha(p->ui.slide_alpha->value());
		setColorInternal(col);
	}
}

void ColorsDialog::set_rgb()
{
	if (!signalsBlocked())
	{
		QColor col(
			p->ui.slide_red->value(),
			p->ui.slide_green->value(),
			p->ui.slide_blue->value(),
			p->ui.slide_alpha->value()
		);
		if (col.saturation() == 0)
			col = QColor::fromHsv(p->ui.slide_hue->value(), 0, col.value());
		p->ui.wheel->setColor(col);
		setColorInternal(col);
	}
}

void ColorsDialog::on_edit_hex_colorChanged(const QColor& color)
{
	setColorInternal(color);
}

void ColorsDialog::on_edit_hex_colorEditingFinished(const QColor& color)
{
	p->ui.edit_hex->setModified(false);
	setColorInternal(color);
}

void ColorsDialog::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasColor() ||
		(event->mimeData()->hasText() && QColor(event->mimeData()->text()).isValid()))
		event->acceptProposedAction();
}

void ColorsDialog::dropEvent(QDropEvent* event) {
	if (event->mimeData()->hasColor())
	{
		setColorInternal(event->mimeData()->colorData().value<QColor>());
		event->accept();
	}
	else if (event->mimeData()->hasText())
	{
		QColor col(event->mimeData()->text());
		if (col.isValid())
		{
			setColorInternal(col);
			event->accept();
		}
	}
}

void ColorsDialog::mouseReleaseEvent(QMouseEvent* event)
{
	if (p->pick_from_screen)
	{
		setColorInternal(utils::get_screen_color(event->globalPosition().toPoint()));
		p->pick_from_screen = false;
		releaseMouse();
	}
}

void ColorsDialog::mouseMoveEvent(QMouseEvent* event)
{
	if (p->pick_from_screen)
	{
		setColorInternal(utils::get_screen_color(event->globalPosition().toPoint()));
	}
}

void ColorsDialog::keyReleaseEvent(QKeyEvent* ev)
{
	QDialog::keyReleaseEvent(ev);

#ifdef Q_OS_ANDROID
	if (ev->key() == Qt::Key_Back)
	{
		reject();
		ev->accept();
	}
#endif
}

void ColorsDialog::setWheelShape(ColorWheel::ShapeEnum shape)
{
	p->ui.wheel->setSelectorShape(shape);
}

ColorWheel::ShapeEnum ColorsDialog::wheelShape() const
{
	return p->ui.wheel->selectorShape();
}

void ColorsDialog::setColorSpace(ColorWheel::ColorSpaceEnum space)
{
	p->ui.wheel->setColorSpace(space);
}

ColorWheel::ColorSpaceEnum ColorsDialog::colorSpace() const
{
	return p->ui.wheel->colorSpace();
}

void ColorsDialog::setWheelRotating(bool rotating)
{
	p->ui.wheel->setRotatingSelector(rotating);
}

bool ColorsDialog::wheelRotating() const
{
	return p->ui.wheel->rotatingSelector();
}

int ColorsDialog::exec()
{
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_FAKE)
	showMaximized();
	setFocus();
#endif
	return QDialog::exec();
}
} // namespace color_widgets