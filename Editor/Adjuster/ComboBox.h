#ifndef ComboBox_h__
#define ComboBox_h__
#include "Adjuster.h"

#include "ExtType/QCombo.h"

class QComboBox;

class ComboBox : public Adjuster
{
	Q_OBJECT
public:
	ComboBox(QCombo value, QWidget* parent = nullptr);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	QCombo value_;
	QComboBox* comboBox_;
};

#endif // ComboBox_h__
