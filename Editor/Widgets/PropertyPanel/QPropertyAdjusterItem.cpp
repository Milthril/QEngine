#include "QPropertyAdjusterItem.h"
#include <QHBoxLayout>
#include <QLabel>
#include "Adjuster/Adjuster.h"

QPropertyAdjusterItem::QPropertyAdjusterItem(const QString& name, Getter getter, Setter setter, Adjuster* adjuster)
	: mItemWidget(new QWidget)
	, mNameLabel(new QLabel(name))
	, mAdjuster(adjuster)
	, mGetter(getter)
{
	QHBoxLayout* layout = new QHBoxLayout(mItemWidget);
	layout->setAlignment(Qt::AlignCenter);
	layout->setContentsMargins(10, 2, 10, 2);
	layout->addWidget(mNameLabel, 100, Qt::AlignLeft);
	layout->addWidget(mAdjuster);
	mItemWidget->setAttribute(Qt::WA_TranslucentBackground, true);
	mNameLabel->setStyleSheet("background-color:rgba(0,0,0,0)");
	adjuster->setItem(this);
	QObject::connect(adjuster, &Adjuster::valueChanged, [this,setter](QVariant var) {
		setter(var);
	});
}

QPropertyAdjusterItem::~QPropertyAdjusterItem()
{
	qDebug() << "release Item";
}

void QPropertyAdjusterItem::createWidgetOrSubItem()
{
	treeWidget()->setItemWidget(this, 0, mItemWidget);
}

void QPropertyAdjusterItem::update()
{
	QVariant var = mGetter();
	if (var != mAdjuster->getValue()) {
		mAdjuster->setValue(var);
	}
}
