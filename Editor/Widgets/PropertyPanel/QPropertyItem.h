#ifndef QPropertyItem_h__
#define QPropertyItem_h__

#include <QMetaProperty>
#include <QTreeWidgetItem>

class QObject;
class QLabel;
class QHBoxLayout;

class QPropertyItem : public QTreeWidgetItem
{
public:
	explicit QPropertyItem(QObject* object, QMetaProperty property);
	~QPropertyItem();
	virtual QWidget* createWidget();
	virtual void setUp(QTreeWidgetItem* tree);
	virtual void setUp(QTreeWidget* tree);
	void rename();
protected:
	QObject* object_ = nullptr;
	QMetaProperty property_;
	QWidget* itemWidget_ = nullptr;
	QHBoxLayout* layout_ = nullptr;
	QLabel* name_ = nullptr;
};

#endif // QPropertyItem_h__