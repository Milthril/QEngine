#include "SceneTreeWidget.h"
#include <memory>
#include <QQueue>
#include "QApplication"

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
		if (oPtr) {
			Q_EMIT objectChanged(oPtr);
		}
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
	QQueue<QPair<QObject*, QTreeWidgetItem*>> qObject;
	qObject.push_back({ mScene.get(),mRoot });
	while (!qObject.isEmpty()) {
		QPair<QObject*, QTreeWidgetItem*> parent = qObject.takeFirst();
		for (auto child : parent.first->children()) {
			QSceneComponent* comp = dynamic_cast<QSceneComponent*>(child);
			if (comp) {
				QTreeWidgetItem* item = new QTreeWidgetItem({ child->objectName() });
				item->setData(1, 0, QVariant::fromValue(child));
				parent.second->addChild(item);
				qObject.push_back({ child,item });
				child->disconnect(this);
				connect(child, &QObject::objectNameChanged, this, [item](const QString& name) {
					item->setText(0, name);
				});
			}
		}
	}
}