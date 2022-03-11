#ifndef RangeSlider_h__
#define RangeSlider_h__

#include "Adjuster.h"
#include "ExtType/QBoundedDouble.h"
#include "ExtType/QRange.h"

class QFocusLineEdit;
class QLabel;
class CustomSlider;

class RangeSlider : public Adjuster
{
	Q_OBJECT
public:
	RangeSlider(QRange range, QWidget* parent = nullptr);
	~RangeSlider();
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QRange range_;
	QFocusLineEdit* lowerEditer_;
	CustomSlider* slider_;
	QFocusLineEdit* upperEditer_;
};

#endif // RangeSlider_h__
