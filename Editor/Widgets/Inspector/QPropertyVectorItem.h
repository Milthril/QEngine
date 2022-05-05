#ifndef QPropertyVectorItem_h__
#define QPropertyVectorItem_h__

#include "QPropertyItem.h"

class QLabel; 
template<typename VectorType>
class QPropertyVectorItem : public QPropertyItem {
public:

	explicit QPropertyVectorItem(const QString& name, Getter getter, Setter setter) : mGetter(getter)
		, mSetter(setter)
		, mItemWidget(new QWidget)
		, mNameLabel(new QLabel(name)) {
		QHBoxLayout* layout = new QHBoxLayout(mItemWidget);
		layout->setAlignment(Qt::AlignCenter);
		layout->setContentsMargins(10, 2, 10, 2);
		layout->addWidget(mNameLabel, 100, Qt::AlignLeft);
		mItemWidget->setAttribute(Qt::WA_TranslucentBackground, true);
		mNameLabel->setStyleSheet("background-color:rgba(0,0,0,0)");
	}

	virtual void createWidgetOrSubItem() {
		treeWidget()->setItemWidget(this, 0, mItemWidget);
		mVector = mGetter().value<VectorType>();
		for (int i = 0; i < mVector.size(); i++) {
			QPropertyItem* listItem = QPropertyItemFactory::instance()->createItem(QMetaType::fromType<decltype(mVector[0])>(),
																				   QString::number(i),
																				   [this, i]() {
																					   return QVariant::fromValue(mVector[i]);
																				   },
																				   [this, i](QVariant var) {
																					   mVector[i] = var.value<std::remove_reference<decltype(mVector[i])>::type>();
																					   mSetter(QVariant::fromValue(mVector));
																				   });
			if (listItem) {
				addChild(listItem);
				listItem->createWidgetOrSubItem();
			}
		}
	}

	virtual void update() override {
		for (int i = 0; i < childCount(); i++) {
			QPropertyItem* item = dynamic_cast<QPropertyItem*>(child(i));
			if (item)
				item->update();
		}
	}

protected:
	QWidget* mItemWidget;
	QLabel* mNameLabel;
	Getter mGetter;
	Setter mSetter;
	VectorType mVector;
};


#endif // QPropertyVectorItem_h__
