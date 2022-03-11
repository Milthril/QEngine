#ifndef GradientDialog_h__
#define GradientDialog_h__

#include <QDialog>
#include "Component\ColorPreview.hpp"
#include "Component\ColorWheel.hpp"
#include "Component\GradientHelper.hpp"
class QAbstractButton;

namespace QWidgetEx {
class ColorsDialog : public QDialog
{
	Q_OBJECT
		Q_ENUMS(ButtonMode)
		Q_PROPERTY(QColor color READ color WRITE setCurrentColor NOTIFY colorChanged DESIGNABLE true)
		Q_PROPERTY(ColorWheel::ShapeEnum wheelShape READ wheelShape WRITE setWheelShape NOTIFY wheelShapeChanged)
		Q_PROPERTY(ColorWheel::ColorSpaceEnum colorSpace READ colorSpace WRITE setColorSpace NOTIFY colorSpaceChanged)
		Q_PROPERTY(bool wheelRotating READ wheelRotating WRITE setWheelRotating NOTIFY wheelRotatingChanged)
		/**
		 * \brief whether the color alpha channel can be edited.
		 *
		 * If alpha is disabled, the selected color's alpha will always be 255.
		 */
		Q_PROPERTY(bool alphaEnabled READ alphaEnabled WRITE setAlphaEnabled NOTIFY alphaEnabledChanged)

public:
	enum ButtonMode {
		OkCancel,
		OkApplyCancel,
		Close
	};

	explicit ColorsDialog(QWidget* parent = 0, Qt::WindowFlags f = {});

	~ColorsDialog();

	/**
	 * Get currently selected color
	 */
	QColor color() const;

	bool alphaEnabled() const;

	QSize sizeHint() const Q_DECL_OVERRIDE;

	ColorWheel::ShapeEnum wheelShape() const;
	ColorWheel::ColorSpaceEnum colorSpace() const;
	bool wheelRotating() const;

	int exec() Q_DECL_OVERRIDE;

public Q_SLOTS:
	void setCurrentColor(const QColor& c);
	void showColor(const QColor& oldcolor);
	void showColors(const QGradientStops& oldColors);
	void setWheelShape(ColorWheel::ShapeEnum shape);
	void setColorSpace(ColorWheel::ColorSpaceEnum space);
	void setWheelRotating(bool rotating);
	void setAlphaEnabled(bool a);
	void setColors(QGradientStops colors);
	QGradientStops colors();
Q_SIGNALS:
	void colorChanged(QColor);
	void colorSelected(QColor);
	void colorsChanged(QGradientStops);

	void wheelShapeChanged(ColorWheel::ShapeEnum shape);
	void colorSpaceChanged(ColorWheel::ColorSpaceEnum space);
	void wheelRotatingChanged(bool rotating);

	void alphaEnabledChanged(bool alphaEnabled);

private Q_SLOTS:
	/// Update all the Ui elements to match the selected color
	void setColorInternal(const QColor& color);
	/// Update from HSV sliders
	void set_hsv();
	/// Update from RGB sliders
	void set_rgb();
	/// Update from Alpha slider
	void set_alpha();

	void on_edit_hex_colorChanged(const QColor& color);
	void on_edit_hex_colorEditingFinished(const QColor& color);

protected:
	void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
	void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

private:
	class Private;
	Private* const p;
};
}

#endif // GradientDialog_h__