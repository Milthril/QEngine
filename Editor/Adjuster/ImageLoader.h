#ifndef ImageLoader_h__
#define ImageLoader_h__

#include "Adjuster.h"
#include <QImage>
#include "Button.h"

class QPushButton;
class QLabel;

class ImageLoader : public Button
{
	Q_OBJECT
public:
	ImageLoader(QImage value, QWidget* parent = nullptr);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	void loadFile();
	QImage data_;
	QPixmap cache_;
	void paintEvent(QPaintEvent* event) override;
};

#endif // ImageLoader_h__
