#ifndef WinCloseButton_h__
#define WinCloseButton_h__

#include "HoverButton.h"
#include "Toolkit\QSvgIcon.h"

class WinCloseButton : public HoverButton {
public:
	WinCloseButton();
protected:
	void paintEvent(QPaintEvent*) override;
private:
	QSvgIcon mIcon;
};

#endif // WinCloseButton_h__
