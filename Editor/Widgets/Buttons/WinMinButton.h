#ifndef WinMinButton_h__
#define WinMinButton_h__

#include "HoverButton.h"

class WinMinButton : public HoverButton {
public:
	WinMinButton();
protected:
	void paintEvent(QPaintEvent*) override;
};

#endif // WinMinButton_h__
