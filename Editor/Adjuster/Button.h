#ifndef Button_h__
#define Button_h__

#include "Adjuster.h"

class QFocusLineEdit;

class Button : public Adjuster
{
	Q_OBJECT
public:
	Button(QWidget* parent = nullptr);
protected:
	virtual void enterEvent(QEnterEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
Q_SIGNALS:
	void clicked();
protected:
	QPoint clickPosition_;
	bool hovered_;
};

#endif // Button_h__
