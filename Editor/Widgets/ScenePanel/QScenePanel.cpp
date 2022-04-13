#include "QApplication"
#include "QEngine.h"
#include "QScenePanel.h"
#include <memory>
#include <QQueue>

QScenePanel::QScenePanel(std::shared_ptr<QWorld> world)
	:mWorld(world)
{
	createUI();
}


void QScenePanel::createUI() {
	setColumnCount(1);
	setIndentation(8);
	setHeaderHidden(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setColumnWidth(0, 120);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setFrameStyle(QFrame::NoFrame);

	connect(this, &QTreeWidget::currentItemChanged, this, [this](QTreeWidgetItem* current, QTreeWidgetItem*) {
		if (current) {
			QEntity* oPtr = current->data(1, 0).value<QEntity*>();
			Q_EMIT entityChanged(oPtr);
			//mScene->setCurrent(dynamic_cast<QSceneComponent*>(oPtr));
		}
	});
	//connect(mScene.get(),&QScene::currentChanged, this, [this](QSceneComponent* comp) {
	//	Q_EMIT objectChanged(comp);
	//});

	//connect(this, &QTreeWidget::itemPressed, this, [](QTreeWidgetItem* item, int) {
	//	if (qApp->mouseButtons() & Qt::RightButton) {
	//		/*show menu*/
	//	}
	//});

	connect(mWorld.get(), &QWorld::worldChanged, this, [this]() {
		updateUI();
	});
}

void QScenePanel::updateUI()
{
	clear();
	mRoot = new QTreeWidgetItem(QStringList("Root"));
	mRoot->setData(1, 0, QVariant::fromValue(mWorld.get()));		//场景作为根节点
	addTopLevelItem(mRoot);
	QQueue<QPair<QEntity*, QTreeWidgetItem*>> qObject;
	for (auto entity : mWorld->findChildren<QEntity*>(QString(),Qt::FindChildOption::FindDirectChildrenOnly)) {
		qObject.push_back({ entity,mRoot });
	}
	while (!qObject.isEmpty()) {
		QPair<QEntity*, QTreeWidgetItem*> parent = qObject.takeFirst();
		if (parent.first) {
			QTreeWidgetItem* item = new QTreeWidgetItem({ parent.first->objectName() });
			item->setData(1, 0, QVariant::fromValue(parent.first));
			parent.second->addChild(item);
			parent.first->disconnect(this);
			connect(parent.first, &QObject::objectNameChanged, this, [item](const QString& name) {
				item->setText(0, name);
			});
			for (auto child : parent.first->findChildren<QEntity*>(QString(), Qt::FindChildOption::FindDirectChildrenOnly)) {
				qObject.push_back({ child,item });
			}
		}
	}
}

void QScenePanel::showEvent(QShowEvent* event) {
	updateUI();
}
