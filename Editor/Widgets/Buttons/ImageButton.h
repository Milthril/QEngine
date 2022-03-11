#ifndef ImageButton_h__
#define ImageButton_h__

#include "HoverButton.h"

class ImageButton : public HoverButton {
public:
	ImageButton(QString path);
	ImageButton(QPixmap image);
protected:
	void paintEvent(QPaintEvent*) override;
	void resizeEvent(QResizeEvent* event) override;
private:
	QPixmap pixmap_;
	QPixmap showMap_;
};

#endif // ImageButton_h__
