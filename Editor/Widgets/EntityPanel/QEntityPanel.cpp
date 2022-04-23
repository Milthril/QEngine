#include "QEntityPanel.h"

#include <QApplication>
#include <QVBoxLayout>
#include "QMenu"

#include "ECS/QEntity.h"
#include "ECS/Component/IComponent.h"
#include "Toolkit/QWidgetShadowMaker.h"
#include "Widgets/PropertyPanel/QPropertyPanel.h"
#include "QWidgetListPanel.h"

QEntityPanel::QEntityPanel(QEntity* entity /*= nullptr*/) 
	: mEntity(entity)
	, btAddComponent(tr("Add Component"))
	, mWidgetPanel(new QWidgetListPanel)
{
	QVBoxLayout* v = new QVBoxLayout(this);
	v->setContentsMargins(5, 5, 5, 5);
	v->addWidget(&lbName);
	v->addWidget(&btAddComponent);
	v->addWidget(mWidgetPanel);
	v->setAlignment(Qt::AlignTop| Qt::AlignHCenter);
	lbName.setAlignment(Qt::AlignCenter);
	connect(&btAddComponent, &QPushButton::clicked, this, [this]() {
		QMenu menu;
		for (auto& name : ComponentFactory::instance()->getComponentList()) {
			menu.addAction(name, [this,name]() {
				mEntity->addComponent(ComponentFactory::instance()->createComponent(name));
				recreatePanel();
			});
		}
		menu.exec(mapToGlobal(btAddComponent.geometry().bottomLeft()));
	});
	connect(mWidgetPanel, &QWidgetListPanel::widgetRemoved, this, [this](QWidget* w) {
		QPropertyPanel* compPanel = dynamic_cast<QPropertyPanel*>(w);
		if (compPanel) {
			mEntity->removeComponent(dynamic_cast<IComponent*>(compPanel->getObject()));
			delete compPanel;
		}
	});
	recreatePanel();

	connect(&lbName, &QLineEdit::returnPressed, this, [this]() {
		mEntity->setObjectName(lbName.text());
		
	});
	recreatePanel();
}

QEntity* QEntityPanel::getEntity() const {
	return mEntity;
}

void QEntityPanel::setEntity(QEntity* val) {
	if (val != mEntity) {
		mEntity = val;
		recreatePanel();
	}
}

void QEntityPanel::recreatePanel() {
	mWidgetPanel->clear();
	lbName.setVisible(mEntity);
	mWidgetPanel->setVisible(mEntity);
	btAddComponent.setVisible(mEntity);
	if (!mEntity) {
		return;
	}
	lbName.setText(mEntity->objectName());
	for (auto compPtr : mEntity->findChildren<IComponent*>(QString(), Qt::FindChildOption::FindDirectChildrenOnly)) {
		QPropertyPanel* compPanel = new QPropertyPanel(compPtr);
		compPanel->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
		compPanel->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		compPanel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		compPanel->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
		mWidgetPanel->addWidget(compPtr->metaObject()->className(), compPanel);
	}
}

void QEntityPanel::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
}