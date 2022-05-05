#include "QInspector.h"
#include <QVBoxLayout>
#include "Toolkit/QWidgetShadowMaker.h"
#include "ECS/QEntity.h"
#include "QEntityPanel.h"
#include "QPropertyPanel.h"

QInspector::QInspector(QObject* object )
	: mObject(object)
	, mLayout(new QVBoxLayout(this))
{
	recreatePanel();
}

QObject* QInspector::getObject() const {
	return mObject;
}

void QInspector::setObject(QObject* val) {
	if (val != mObject) {
		mObject = val;
		recreatePanel();
	}
}

void QInspector::recreatePanel() {
	if (mWidget) {
		mLayout->removeWidget(mWidget);
		mWidget->setParent(nullptr);
		mWidget->deleteLater();
	}
	if (mObject == nullptr)
		return;
	QEntity* entity = dynamic_cast<QEntity*>(mObject);
	if (entity) {
		QEntityPanel* entityPanel = new QEntityPanel();
		mWidget = entityPanel;
		mLayout->addWidget(mWidget);
		entityPanel->setEntity(entity);
	}
	else {
		QPropertyPanel* propertyPanel = new QPropertyPanel();
		mWidget = propertyPanel;
		mLayout->addWidget(mWidget);
		propertyPanel->setObject(mObject);
	}

}

