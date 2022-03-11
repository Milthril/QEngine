#ifndef LineEdit_h__
#define LineEdit_h__

#include "Adjuster/Adjuster.h"

class QFocusLineEdit;

class LineEdit : public Adjuster
{
	Q_OBJECT
public:
	LineEdit(QString str);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QFocusLineEdit* lineEdit_;
};

#endif // LineEdit_h__
