#ifndef QIntBox_h__
#define QIntBox_h__

#include "Adjuster.h"
#include "Toolkit\QSvgIcon.h"

class QFocusLineEdit;
class QLabel;

class IntBox : public Adjuster
{
	Q_OBJECT
public:
	IntBox(int value = 0, QString name = "", QWidget* parent = nullptr);
	~IntBox();
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
	QLabel* nameLabel_;
	QFocusLineEdit* numberEditer_;
	QLabel* arrowLabel_;
	QPointF clickPosition_;
	QSvgIcon mSvgIcon;
};

#endif // QIntBox_h__
