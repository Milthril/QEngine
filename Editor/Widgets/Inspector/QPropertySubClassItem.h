#ifndef QPropertySubClassItem_h__
#define QPropertySubClassItem_h__

#include "ExtType\QSubClass.h"
#include "QLabel"
#include "QPropertyItem.h"
#include <functional>
#include <QComboBox>
#include <QHBoxLayout>
#include "QPropertyPanel.h"
#include "QPropertyItemFactory.h"

template<typename _Ty>
class QPropertySubClassItem : public QPropertyItem
{
public:
	using SubClassType = _Ty;
	explicit QPropertySubClassItem(const QString& name, Getter getter, Setter setter)
		: mItemWidget(new QWidget)
		, mNameLabel(new QLabel(name))
		, mComboBox(new QComboBox)
	{
		mSubClass = getter().value<SubClassType>();

		QHBoxLayout* layout = new QHBoxLayout(mItemWidget);
		layout->setAlignment(Qt::AlignCenter);
		layout->setContentsMargins(10, 2, 10, 2);
		layout->addWidget(mNameLabel, 100, Qt::AlignLeft);
		mComboBox->addItems(mSubClass.getSubList());
		mComboBox->setCurrentText(mSubClass.getCurrentClass());
		layout->addWidget(mComboBox);
		mItemWidget->setAttribute(Qt::WA_TranslucentBackground, true);
		mNameLabel->setStyleSheet("background-color:rgba(0,0,0,0)");
		QObject::connect(mComboBox, &QComboBox::currentTextChanged, [this,setter](QString text) {
			bool ret = mSubClass.setSubClass(text);
			setter(QVariant::fromValue<>(mSubClass));
			if (ret) {
				this->takeChildren();
				createWidgetOrSubItem();
			}
		});
	}

	~QPropertySubClassItem()
	{
	}

	virtual void createWidgetOrSubItem()
	{
		treeWidget()->setItemWidget(this, 0, mItemWidget);
		if (mSubClass == nullptr)
			return;
		for (int i = 1; i < mSubClass->metaObject()->propertyCount(); i++) {
			QMetaProperty property = mSubClass->metaObject()->property(i);
			if (!property.isDesignable())
				continue;
			QPropertyItem* item = QPropertyItemFactory::instance()->createItem(property.metaType(),
																			   property.name(),
																			   [this, property]() {return property.read(mSubClass.get()); },
																			   [this, property](QVariant var) { property.write(mSubClass.get(), var); }
			);
			if (item) {
				addChild(item);
				item->createWidgetOrSubItem();
			}
		}
	}

protected:
	SubClassType mSubClass;
	QWidget* mItemWidget;
	QLabel* mNameLabel;
	QComboBox* mComboBox;
};

#endif // QPropertySubClassItem_h__
