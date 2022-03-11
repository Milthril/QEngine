#ifndef ColorButton_h__
#define ColorButton_h__

#include <QPushButton>
#include "ExtType/QColors.h"
#include "Adjuster\Button.h"

class ColorButton : public Button {
public:
	ColorButton(QColor colors);
	void setColor(QColor colors);

	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QColor color_;
};

#endif // ColorButton_h__