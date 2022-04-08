#include "QPropertyPanel.h"
#include <QApplication>
#include <QMetaProperty>
#include "QPropertyItem.h"
#include "QFile"
#include "QPropertyItemFactory.h"
#include "Toolkit\QWidgetShadowMaker.h"

QPropertyPanel::QPropertyPanel(QObject* object /*= nullptr*/) {
	setObject(object);
	setColumnCount(1);
	setIndentation(8);
	setHeaderHidden(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setColumnWidth(0, 120);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setFrameStyle(QFrame::NoFrame);
	setGraphicsEffect(new QWidgetShadowMaker);
	connect(this, &QTreeWidget::itemPressed, this, [](QTreeWidgetItem* item, int) {
		if (qApp->mouseButtons() & Qt::RightButton) {
			/*show menu*/
		}
	});
}

QObject* QPropertyPanel::getObject() const
{
	return mObject;
}

void QPropertyPanel::setObject(QObject* val) {
	if (val != mObject) {
		mObject = val;
		if (isVisible())
			recreatePanel();
	}
}

void QPropertyPanel::setupObjectToItem(QTreeWidgetItem* parentItem, QObject* object)
{
	if (object == nullptr)
		return;
	for (int i = 1; i < object->metaObject()->propertyCount(); i++) {
		QMetaProperty property = object->metaObject()->property(i);
		if (property.isDesignable()) {
			QPropertyItem* item = QPropertyItemFactory::instance()->createItem(property.typeId(),
																			   property.name(),
																			   [object, property]() {return property.read(object); },
																			   [object, property](QVariant var) { property.write(object, var); }
			);
			if (item) {
				parentItem->addChild(item);
				item->createWidgetOrSubItem();
				const QMetaObject* meta = property.metaType().metaObject();
				if (meta != nullptr && meta->inherits(&QObject::staticMetaObject)) {
					QObject* obj = property.read(object).value<QObject*>();
					if (obj != nullptr) {
						setupObjectToItem(item, obj);
					}
				}
			}
		}
	}
}

void QPropertyPanel::recreatePanel() {
	this->clear();
	if (mObject == nullptr)
		return;
	for (int i = 1; i < mObject->metaObject()->propertyCount(); i++) {
		QMetaProperty property = mObject->metaObject()->property(i);
		if (!property.isDesignable())
			continue;
		QPropertyItem* item = QPropertyItemFactory::instance()->createItem(property.typeId(),
																		   property.name(),
																		   [this, property]() {return property.read(mObject); },
																		   [this, property](QVariant var) { property.write(mObject, var); }
		);
		if (item) {
			addTopLevelItem(item);
			item->createWidgetOrSubItem();
		}
		else {
			if (QMetaType::canConvert(property.metaType(), QMetaType::fromType<QObject*>())) {		//处理Object*属性
				QObject* obj = property.read(mObject).value<QObject*>();
				if (obj != nullptr) {
					QTreeWidgetItem* item = new QTreeWidgetItem;
					item->setText(0, property.name());
					addTopLevelItem(item);
					setupObjectToItem(item, obj);
				}

			}
			if (QMetaType::canConvert(property.metaType(), QMetaType::fromType<QVariantList>())) {	//处理数组类型
				QTreeWidgetItem* item = new QTreeWidgetItem;
				item->setText(0, property.name());
				addTopLevelItem(item);
				QVariantList varList = property.read(mObject).toList();
				for (int i = 0; i < varList.size(); i++) {
					QPropertyItem* listItem = QPropertyItemFactory::instance()->createItem(varList[i].typeId(),
																					   QString::number(i),
						[this, property, i]() {
							return property.read(mObject).toList()[i];
						},
						[this, property, i](QVariant var) {
							QVariantList varList = property.read(mObject).toList();
							varList[i] = var;
							property.write(mObject, varList);
						});
					if (listItem) {
						item->addChild(listItem);
						listItem->createWidgetOrSubItem();
					}
				}
			}
		}
	}
	this->expandAll();

	//for (auto& dpName : mObject->dynamicPropertyNames()) {
	//	QDynamicPropertyItem* item = new QDynamicPropertyItem(mObject, dpName);
	//	item->setUp(this);
	//	QVariant var = mObject->property(dpName);
	//	const QMetaObject* meta = var.metaType().metaObject();
	//	if (meta != nullptr && meta->inherits(&QObject::staticMetaObject)) {
	//		QObject* obj = var.value<QObject*>();
	//		if (obj != nullptr) {
	//			setupItemInternal(obj, item);
	//		}
	//	}
	//}
}

void QPropertyPanel::updatePanel()
{
	QTreeWidgetItemIterator iter(this);
	while (*iter) {
		QPropertyItem* item = dynamic_cast<QPropertyItem*>(*iter);
		if (item) {
			item->update();
		}
		iter++;
	}
}

void QPropertyPanel::showEvent(QShowEvent* event)
{
	QTreeWidget::showEvent(event);
	recreatePanel();
}