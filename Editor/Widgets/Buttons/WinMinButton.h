#ifndef WinMinButton_h__
#define WinMinButton_h__

#include "HoverButton.h"
#include "Toolkit\QSvgIcon.h"

class WinMinButton : public HoverButton {
public:
	WinMinButton();
protected:
	void paintEvent(QPaintEvent*) override;
private:
	QSvgIcon mIcon;
};

#endif // WinMinButton_h__
