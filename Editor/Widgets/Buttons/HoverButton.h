#ifndef HoverButton_h__
#define HoverButton_h__
#include <QPushButton>

class HoverButton :public QPushButton {
public:
	HoverButton();
	QColor getCheckColor() const { return checkColor; }
	void setCheckColor(QColor val) { checkColor = val; }
	QColor getHoverColor() const { return hoverColor; }
	void setHoverColor(QColor val) { hoverColor = val; update(); }
protected:
	virtual void enterEvent(QEnterEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;
	virtual void paintEvent(QPaintEvent*) override;
protected:
	bool hovered = false;
	QColor checkColor = QColor(230, 230, 230, 240);
	QColor hoverColor = QColor(220, 220, 220, 240);

};

#endif // WinMinButton_h__