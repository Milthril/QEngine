#ifndef QPropertyItemFactory_h__
#define QPropertyItemFactory_h__

#include <functional>
#include <QVariant>
#include "QPropertyItem.h"

class QPropertyItemFactory {
public:
	using Getter = QPropertyItem::Getter;
	using Setter = QPropertyItem::Setter;
	using ItemCreator = std::function<QPropertyItem* (QString, Getter, Setter)>;
	static QPropertyItemFactory* instance();
	QPropertyItem* createItem(QMetaType metaType, QString name, Getter getter, Setter setter);
private:
	QPropertyItemFactory();
private:
	QHash<int, ItemCreator> mCreatorMap;
};


#endif // QPropertyItemFactory_h__