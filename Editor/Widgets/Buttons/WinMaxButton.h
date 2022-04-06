#ifndef WinMaxButton_h__
#define WinMaxButton_h__

#include "HoverButton.h"
#include "Toolkit\QSvgIcon.h"

class WinMaxButton : public HoverButton {
public:
	WinMaxButton();
protected:
	void paintEvent(QPaintEvent*) override;
	void updateTopLevelWidget();
private:
	QSvgIcon mFullScreen;
	QSvgIcon mWindow;
	QWidget* mTopLevelWidget = nullptr;
};

#endif // WinMaxButton_h__