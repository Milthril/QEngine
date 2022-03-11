#ifndef ColorsButton_h__
#define ColorsButton_h__

#include <QPushButton>
#include "ExtType/QColors.h"
#include "Adjuster/Button.h"

class ColorsButton : public Button {
public:
	ColorsButton(QColors colors);
	void setColors(QColors colors);

	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QColors colors_;
};

#endif // ColorsButton_h__