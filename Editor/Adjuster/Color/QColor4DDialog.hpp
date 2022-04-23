#ifndef QColor4DDialog_h__
#define QColor4DDialog_h__

#include "Component\ColorLineEdit.hpp"
#include "Component\ColorPreview.hpp"
#include "Component\ColorWheel.hpp"
#include "Component\QGradientSlider.hpp"
#include "QPushButton"
#include "ExtType\QColor4D.h"
#include <QDialog>

class QColor4DDialog :public QDialog {
	Q_OBJECT
public:
	QColor4DDialog();
	void setColor(QColor4D color);
	int exec(QColor4D color);
private:
	void createUI();
	void connectUI();
	void setCurrentColorInternal(QColor4D color);
Q_SIGNALS:
	void colorChanged(QColor4D);
private:
	QWidgetEx::ColorWheel* wheel;
	QWidgetEx::ColorPreview* preview;
	QGradientSlider* sliderRed;
	QGradientSlider* sliderGreen;
	QGradientSlider* sliderBlue;
	QGradientSlider* sliderAlpha;
	QGradientSlider* sliderHue;
	QGradientSlider* sliderSaturation;
	QGradientSlider* sliderValue;
	QPushButton* btOk;
	QPushButton* btCancel;
	QWidgetEx::ColorLineEdit* leHex;
	QColor4D current;
	QColor4D pre;
};

#endif // QColor4DDialog_h__