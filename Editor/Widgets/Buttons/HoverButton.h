#ifndef HoverButton_h__
#define HoverButton_h__
#include <QPushButton>

class  HoverButton :public QPushButton {
protected:
	virtual void enterEvent(QEnterEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;
	virtual void paintEvent(QPaintEvent*) override;
protected:
	bool hovered = false;
	QColor hoverColor = QColor(220, 220, 220);
public:
	QColor getHoverColor() const { return hoverColor; }
	void setHoverColor(QColor val) { hoverColor = val; update(); }
};

#endif // WinMinButton_h__