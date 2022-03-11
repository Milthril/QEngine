#ifndef DoubleSlider_h__
#define DoubleSlider_h__

#include "Adjuster.h"
#include "ExtType/QBoundedDouble.h"

class QFocusLineEdit;
class QLabel;

class DoubleSlider : public Adjuster
{
	Q_OBJECT
public:
	DoubleSlider(QBoundedDouble number = QBoundedDouble(0, 0, 100), QString name = "", QWidget* parent = nullptr);
	~DoubleSlider();
	double number();
	void setNumber(double num);
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
	QBoundedDouble value_;
	QLabel* nameLabel_;
	QFocusLineEdit* numberEditer_;
	QLabel* arrowLabel_;
	QPointF clickPosition_;
	QBrush mBrush = QColor(200,200,200);
};

#endif // DoubleSlider_h__