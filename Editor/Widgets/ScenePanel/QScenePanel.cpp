#include "QApplication"
#include "QEngine.h"
#include "Scene/Component/QPrimitiveComponent.h"
#include "QScenePanel.h"
#include <memory>
#include <QQueue>
#include "Scene/QSceneComponent.h"
#include "Renderer/CommonPass/DebugDrawPass.h"

QScenePanel::QScenePanel(std::shared_ptr<QScene> scene)
	:mScene(scene)
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
		QObject* oPtr = current->data(1, 0).value<QObject*>();
		//Engine->debugPainter()->setCurrentCompInternal(dynamic_cast<QSceneComponent*>(oPtr));
		Q_EMIT objectChanged(oPtr);
	});

	connect(Engine->debugPainter().get(), &DebugDrawPass::currentCompChanged, this, [this](QSceneComponent* comp) {
		QTreeWidgetItemIterator iter(this);
		while (*iter) {
			QObject* oPtr = (*iter)->data(1, 0).value<QObject*>();
			if (oPtr == comp) {
				setCurrentItem(*iter);
				return;
			}
			iter++;
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

void QScenePanel::updateUI()
{
	clear();
	mRoot = new QTreeWidgetItem(QStringList("Root"));
	mRoot->setData(1, 0, QVariant::fromValue(mScene.get()));		//场景作为根节点
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