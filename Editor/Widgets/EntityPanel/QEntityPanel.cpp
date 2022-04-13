#include "QEntityPanel.h"
#include <QApplication>
#include "Toolkit\QWidgetShadowMaker.h"
#include <QVBoxLayout>
#include "ECS/QEntity.h"
#include "..\PropertyPanel\QPropertyPanel.h"

QEntityPanel::QEntityPanel(QEntity* entity /*= nullptr*/) 
	: mEntity(entity)
	, btAddComponent(tr("Add Component"))
{
	QVBoxLayout* v = new QVBoxLayout(this);
	v->setContentsMargins(5, 5, 5, 5);
	v->addWidget(&lbName);
	v->addWidget(&btAddComponent);
	v->addWidget(&mToolBox);

	v->setAlignment(Qt::AlignTop| Qt::AlignHCenter);
	lbName.setAlignment(Qt::AlignCenter);
}

QEntity* QEntityPanel::getEntity() const {
	return mEntity;
}

void QEntityPanel::setEntity(QEntity* val) {
	if (val != mEntity) {
		mEntity = val;
		if (isVisible())
			recreatePanel();
	}
}

void QEntityPanel::clearToolBox() {
	while (mToolBox.count()) {
		mToolBox.removeItem(0);
	}
}

void QEntityPanel::recreatePanel() {
	lbName.setVisible(mEntity);
	mToolBox.setVisible(mEntity);
	btAddComponent.setVisible(mEntity);
	if (!mEntity) {
		return;
	}
	clearToolBox();
	lbName.setText(mEntity->objectName());
	for (auto compPtr : mEntity->findChildren<IComponent*>(QString(), Qt::FindChildOption::FindDirectChildrenOnly)) {
		QPropertyPanel* compPanel = new QPropertyPanel(compPtr);
		mToolBox.addItem(compPanel, compPtr->metaObject()->className());
	}
}

void QEntityPanel::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	recreatePanel();
}