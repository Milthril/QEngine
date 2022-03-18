#ifndef QPropertySubClassItem_h__
#define QPropertySubClassItem_h__

#include "ExtType\QSubClass.h"
#include "QLabel"
#include "QPropertyItem.h"
#include <functional>
#include <QComboBox>
#include <QHBoxLayout>
#include "QPropertyPanel.h"

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
		layout->addWidget(mComboBox);
		mItemWidget->setAttribute(Qt::WA_TranslucentBackground, true);
		mNameLabel->setStyleSheet("background-color:rgba(0,0,0,0)");
	}

	~QPropertySubClassItem()
	{
	}

	virtual void createWidgetOrSubItem()
	{
		treeWidget()->setItemWidget(this, 0, mItemWidget);
		QPropertyPanel::setupObjectToItem(this, mSubClass.get());
	}

protected:
	SubClassType mSubClass;
	QWidget* mItemWidget;
	QLabel* mNameLabel;
	QComboBox* mComboBox;
};

#endif // QPropertySubClassItem_h__
