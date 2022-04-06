#ifndef IntSlider_h__
#define IntSlider_h__

#include "Adjuster.h"
#include "ExtType/QBoundedInt.h"
#include "Toolkit/QSvgIcon.h"

class QFocusLineEdit;
class QLabel;

class IntSlider : public Adjuster
{
	Q_OBJECT
public:
	IntSlider(QBoundedInt number = QBoundedInt(0, 0, 100), QString name = "", QWidget* parent = nullptr);
	~IntSlider();
	int number();
	void setNumber(int num);
	void setEditEnabled(bool enable);
	bool getEditEnabled();

	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	void moveBox(QPointF offset);
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
private:
	QBoundedInt value_;
	QLabel* nameLabel_;
	QFocusLineEdit* numberEditer_;
	QLabel* arrowLabel_;
	QPointF clickPosition_;
	QSvgIcon mSvgIcon;
};

#endif // IntSlider_h__