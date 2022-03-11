#ifndef WinCloseButton_h__
#define WinCloseButton_h__

#include "HoverButton.h"

class WinCloseButton : public HoverButton {
public:
	WinCloseButton();
protected:
	void paintEvent(QPaintEvent*) override;
};

#endif // WinCloseButton_h__
