#ifndef Adjuster_h__
#define Adjuster_h__

#include <QWidget>
#include <QVariant>

class QPropertyAdjusterItem;

class Adjuster :public QWidget {
	Q_OBJECT
public:
	Adjuster(QWidget* parent = nullptr) {}
	virtual QVariant getValue() = 0;
	virtual void setValue(QVariant var) = 0;
	Q_INVOKABLE virtual void updateValue(QVariant var) {
		if (var != getValue()) {
			setValue(var);
		}
	}
	QPropertyAdjusterItem* getItem() const { return mItem; }
	void setItem(QPropertyAdjusterItem* val) { mItem = val; }
protected:
	QPropertyAdjusterItem* mItem = nullptr;
Q_SIGNALS:
	void valueChanged(QVariant);
};

#endif // Adjuster_h__
