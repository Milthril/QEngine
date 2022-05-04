#include "QPropertyObjectItem.h"
#include "QPropertyItemFactory.h"
#include "QBoxLayout"

QPropertyObjectItem::QPropertyObjectItem(const QString& name, QObject* object) 
	: mItemWidget(new QWidget)
	, mNameLabel(new QLabel(name))
	, mObject(object){

	QHBoxLayout* layout = new QHBoxLayout(mItemWidget);
	layout->setAlignment(Qt::AlignCenter);
	layout->setContentsMargins(10, 2, 10, 2);
	layout->addWidget(mNameLabel, 100, Qt::AlignLeft);
	mItemWidget->setAttribute(Qt::WA_TranslucentBackground, true);
	mNameLabel->setStyleSheet("background-color:rgba(0,0,0,0)");
}

void QPropertyObjectItem::createWidgetOrSubItem() {
	treeWidget()->setItemWidget(this, 0, mItemWidget);
	if (mObject == nullptr)
		return;
	for (int i = 1; i < mObject->metaObject()->propertyCount(); i++) {
		QMetaProperty property = mObject->metaObject()->property(i);
		if (!property.isDesignable())
			continue;
		QPropertyItem* item = QPropertyItemFactory::instance()->createItem(property.metaType(),
																		   property.name(),
																		   [this, property]() {return property.read(mObject); },
																		   [this, property](QVariant var) { property.write(mObject, var); }
		);
		if (item) {
			addChild(item);
			item->createWidgetOrSubItem();
		}
	}
}

void QPropertyObjectItem::update() {
	for (int i = 0; i < childCount(); i++) {
		QPropertyItem* item = dynamic_cast<QPropertyItem*>(child(i));
		if (item)
			item->update();
	}
}

