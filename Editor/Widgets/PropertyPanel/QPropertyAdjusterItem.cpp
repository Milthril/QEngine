#include "QPropertyAdjusterItem.h"
#include <QHBoxLayout>
#include <QLabel>
#include "Adjuster/Adjuster.h"

QPropertyAdjusterItem::QPropertyAdjusterItem(const QString& name, Getter getter, Setter setter, Adjuster* adjuster)
	: mItemWidget(new QWidget)
	, mNameLabel(new QLabel(name))
	, mAdjuster(adjuster)
{
	QHBoxLayout* layout = new QHBoxLayout(mItemWidget);
	layout->setAlignment(Qt::AlignCenter);
	layout->setContentsMargins(10, 2, 10, 2);
	layout->addWidget(mNameLabel, 100, Qt::AlignLeft);
	layout->addWidget(mAdjuster);
	mItemWidget->setAttribute(Qt::WA_TranslucentBackground, true);
	mNameLabel->setStyleSheet("background-color:rgba(0,0,0,0)");

	QObject::connect(adjuster, &Adjuster::valueChanged, [this,setter](QVariant var) {
		setter(var);
	});
}

QPropertyAdjusterItem::~QPropertyAdjusterItem()
{
	if (mItemWidget)
		mItemWidget->deleteLater();
}

void QPropertyAdjusterItem::createWidgetOrSubItem()
{
	treeWidget()->setItemWidget(this, 0, mItemWidget);
}