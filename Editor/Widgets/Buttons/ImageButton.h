#ifndef ImageButton_h__
#define ImageButton_h__

#include "HoverButton.h"
#include "Toolkit\QSvgIcon.h"

class ImageButton : public HoverButton {
public:
	ImageButton(QString path);
protected:
	void paintEvent(QPaintEvent*) override;
private:
	QSvgIcon mIcon;
};

#endif // ImageButton_h__
