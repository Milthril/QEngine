#include "SceneTreeWidget.h"
#include <memory>
#include <QQueue>
#include "QApplication"
#include "Scene/Component/QPrimitiveComponent.h"

SceneTreeWidget::SceneTreeWidget(std::shared_ptr<QScene> scene)
	:mScene(scene)
{
	createUI();
}

void SceneTreeWidget::createUI() {
	setColumnCount(1);
	setIndentation(8);
	setHeaderHidden(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setColumnWidth(0, 120);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setFrameStyle(QFrame::NoFrame);

	connect(this, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* item, int) {
		QObject* oPtr = item->data(1, 0).value<QObject*>();
		Q_EMIT objectChanged(oPtr);
	});

	connect(this, &QTreeWidget::itemPressed, this, [](QTreeWidgetItem* item, int) {
		if (qApp->mouseButtons() & Qt::RightButton) {
			/*show menu*/
		}
	});

	connect(mScene.get(), &QScene::primitiveInserted, this, [this](int, std::shared_ptr<QPrimitiveComponent>) {
		updateUI();
	});
}

void SceneTreeWidget::updateUI()
{
	clear();
	mRoot = new QTreeWidgetItem(QStringList("Root"));
	mRoot->setData(1, 0, QVariant::fromValue(mRoot));
	addTopLevelItem(mRoot);
	QQueue<QPair<std::shared_ptr<QSceneComponent>, QTreeWidgetItem*>> qObject;
	for (auto comp : mScene->geyPrimitiveList()) {
		qObject.push_back({ comp,mRoot });
	}

	while (!qObject.isEmpty()) {
		QPair<std::shared_ptr<QSceneComponent>, QTreeWidgetItem*> parent = qObject.takeFirst();
		if (parent.first) {
			QTreeWidgetItem* item = new QTreeWidgetItem({ parent.first->objectName() });
			item->setData(1, 0, QVariant::fromValue(parent.first.get()));
			parent.second->addChild(item);

			parent.first->disconnect(this);
			connect(parent.first.get(), &QObject::objectNameChanged, this, [item](const QString& name) {
				item->setText(0, name);
			});
			for (auto child : parent.first->getChildren()) {
				qObject.push_back({ child,item });
			}
		}
	}
}