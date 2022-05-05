#ifndef QPropertyObjectItem_h__
#define QPropertyObjectItem_h__

#include "QPropertyItem.h"
#include "QLabel"

class QPropertyObjectItem : public QPropertyItem {
public:
	explicit QPropertyObjectItem(const QString& name, QObject* object);
	virtual void createWidgetOrSubItem();
	void update() override;
protected:
	QWidget* mItemWidget;
	QLabel* mNameLabel;
	QObject* mObject = nullptr;
};


#endif // QPropertyObjectItem_h__