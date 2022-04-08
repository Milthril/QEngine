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
	using Getter = std::function<QVariant()>;
	using Setter = std::function<void(QVariant)>;
	explicit QPropertyItem();
	~QPropertyItem();
	virtual void createWidgetOrSubItem() = 0;
	virtual void update() {};
};

#endif // QPropertyItem_h__