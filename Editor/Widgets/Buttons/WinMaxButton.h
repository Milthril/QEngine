#ifndef WinMaxButton_h__
#define WinMaxButton_h__

#include "HoverButton.h"

class WinMaxButton : public HoverButton {
public:
	WinMaxButton();
protected:
	void paintEvent(QPaintEvent*) override;
};

#endif // WinMaxButton_h__