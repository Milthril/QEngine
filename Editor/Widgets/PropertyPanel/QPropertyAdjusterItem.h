#ifndef QPropertyAdjusterItem_h__
#define QPropertyAdjusterItem_h__

#include "QPropertyItem.h"
#include "QWidget"

class QLabel;
class QString;
class Adjuster;

class QPropertyAdjusterItem : public QPropertyItem
{
public:
	explicit QPropertyAdjusterItem(const QString& name, Getter getter, Setter setter, Adjuster* adjuster);
	~QPropertyAdjusterItem();
	virtual void createWidgetOrSubItem();
protected:
	QWidget* mItemWidget;
	QLabel* mNameLabel;
	Adjuster* mAdjuster;
};

#endif // QPropertyAdjusterItem_h__