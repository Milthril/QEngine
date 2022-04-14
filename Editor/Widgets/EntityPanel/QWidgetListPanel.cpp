#include "QWidgetListPanel.h"
#include <QVBoxLayout>
#include "Widgets\Buttons\WinCloseButton.h"

QWidgetListItem::QWidgetListItem(QString name, QWidget* widget) 
	: mWidget(widget)
	, mTitleButton(name)
	, mCloseButton(new WinCloseButton)
{
	setObjectName("ComponentItem");
	setAttribute(Qt::WA_StyledBackground);
	setAutoFillBackground(true);
	QVBoxLayout* v = new QVBoxLayout(this);
	QHBoxLayout* h = new QHBoxLayout;
	h->setContentsMargins(5, 5, 5, 5);
	h->setSpacing(0);
	h->addWidget(&mTitleButton);
	h->addWidget(mCloseButton);
	v->setContentsMargins(0, 0, 0, 0);
	v->setSpacing(0);
	v->addLayout(h);
	v->addWidget(mWidget);
	mCloseButton->setFixedSize(15, 15);
	connect(&mTitleButton, &QPushButton::clicked, this, [this]() {
		mWidget->setVisible(!mWidget->isVisible());
	});
	connect(mCloseButton, &QPushButton::clicked, this, [this]() {
		requsetRemoveItem(this);
	});
}

QWidgetListPanel::QWidgetListPanel()
	: mBody(new QWidget) {
	vLayout = new QVBoxLayout(mBody);
	vLayout->setAlignment(Qt::AlignTop);
	setWidget(mBody);
	setWidgetResizable(true);
}

void QWidgetListPanel::clear() {
	QLayoutItem* child;
	while ((child = vLayout->takeAt(0)) != 0) {
		if (child->widget()) {
			child->widget()->setParent(nullptr);
		}
		delete child;
	}
}

void QWidgetListPanel::addWidget(QString name, QWidget* widget) {
	QWidgetListItem* item = new QWidgetListItem(name,widget);
	vLayout->addWidget(item);
	connect(item, &QWidgetListItem::requsetRemoveItem, this, [this,widget](QWidgetListItem* item) {
		vLayout->removeWidget(item);
		item->setParent(nullptr);
		Q_EMIT widgetRemoved(widget);
	});
}

void QWidgetListPanel::removeWidget(QWidget* widget) {

}

