#ifndef QDynamicPropertyItem_h__
#define QDynamicPropertyItem_h__

#include <QMetaProperty>
#include <QTreeWidgetItem>

class QLabel;
class QHBoxLayout;

class QDynamicPropertyItem : public QTreeWidgetItem
{
public:
	explicit QDynamicPropertyItem(QObject* object, QString propertyName);
	~QDynamicPropertyItem();
	virtual QWidget* createWidget();
	virtual void setUp(QTreeWidgetItem* tree);
	virtual void setUp(QTreeWidget* tree);
	void rename();
protected:
	QObject* object_ = nullptr;
	QString propertyName_;
	QWidget* itemWidget_ = nullptr;
	QHBoxLayout* layout_ = nullptr;
	QLabel* name_ = nullptr;
};

#endif // QDynamicPropertyItem_h__