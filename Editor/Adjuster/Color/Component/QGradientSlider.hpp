#ifndef QGradientSlider_h__
#define QGradientSlider_h__

#include <QWidget>
#include "Adjuster\Adjuster.h"
#include "Adjuster\QFocusLineEdit.h"
#include "Toolkit\QSvgIcon.h"

class QFocusLineEdit;
class QLabel;

class QGradientSlider : public Adjuster
{
	Q_OBJECT
public:
	QGradientSlider(QString name = "", float var = 0.0f, float min = 0.0f, float max = 1.0f, bool limited = false, QWidget* parent = nullptr);
	~QGradientSlider();
	float number();
	void setNumber(float num);
	void setEditEnabled(bool enable);
	bool getEditEnabled();
	QVariant getValue() override;
	void setValue(QVariant var) override;
	void setBrush(QBrush val) { mBrush = val; }
protected:
	void moveBox(QPointF offset);
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
protected:
	float var_;
	float min_;
	float max_;
	bool limited_;
	QLabel* nameLabel_;
	QFocusLineEdit* numberEditer_;
	QLabel* arrowLabel_;
	QSvgIcon mSvgIcon;
	QPointF clickPosition_;
	QBrush mBrush = QColor(200, 200, 200);
};

#endif // QGradientSlider_h__