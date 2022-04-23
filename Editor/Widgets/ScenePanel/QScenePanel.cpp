#include "QApplication"
#include "QScenePanel.h"
#include <memory>
#include <QQueue>
#include <QMouseEvent>
#include <QMenu>
#include "QTreeWidgetItemIterator"

QScenePanel::QScenePanel(QWorld* world)
	:mWorld(world)
{
	createUI();
}

void QScenePanel::createUI() {
	setColumnCount(1);
	setIndentation(8);
	setHeaderHidden(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setColumnWidth(0, 120);
	setFrameStyle(QFrame::NoFrame);
	//setDragDropMode(QAbstractItemView::InternalMove);
	//setDefaultDropAction(Qt::DropAction::MoveAction);
	connect(this, &QTreeWidget::currentItemChanged, this, [this](QTreeWidgetItem* current, QTreeWidgetItem*) {
		if (current) {
			QEntity* oPtr = current->data(1, 0).value<QEntity*>();
			mWorld->setCurrentEntity(oPtr);
			Q_EMIT entityChanged(oPtr);
		}
	});
	//connect(this->model(), &QAbstractItemModel::rowsAboutToBeMoved, this, [this](const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow) {
	//	qDebug() << sourceParent;
	//		});	
	//connect(this->model(), &QAbstractItemModel::rowsInserted, this, [this](const QModelIndex& parent, int first, int last) {
	//	qDebug() << parent << first << last;
	//});

	connect(mWorld,&QWorld::currentEntityChanged, this, [this]() {
		if (this->currentItem()) {
			QEntity* entity = currentItem()->data(1, 0).value<QEntity*>();
			if (entity == mWorld->getCurrentEntity())
				return;
		}
		QTreeWidgetItemIterator iter(this);
		while (*iter) {
			QEntity* entity = (*iter)->data(1, 0).value<QEntity*>();
			if (entity&& entity == mWorld->getCurrentEntity()) {
				setCurrentItem(*iter);
				return;
			}
			iter++;
		}
	});

	connect(this, &QTreeWidget::itemPressed, this, [this](QTreeWidgetItem* item, int column) {
		if (qApp->mouseButtons() & Qt::RightButton) {
			QMenu menu;
			menu.addAction("Rename", [this, item, column]() {
				this->editItem(item, column);
			});
			menu.addAction("Remove", [this,column]() {
				const auto& items = this->selectedItems();
				mWorld->blockSignals(true);
				for (auto& item : items) {
					if (currentItem() == item) {
						Q_EMIT entityChanged(nullptr);
					}
					mWorld->removeEntity(item->data(1, 0).value<QEntity*>());
				}
				mWorld->blockSignals(false);
				Q_EMIT mWorld->worldChanged();
			});
			menu.exec(QCursor::pos());
		}
	});

	connect(mWorld, &QWorld::worldChanged, this, [this]() {
		updateUI();
	});
}

void QScenePanel::updateUI()
{
	clear();
	QQueue<QPair<QEntity*, QTreeWidgetItem*>> qObject;
	for (auto entity : mWorld->findChildren<QEntity*>(QString(),Qt::FindChildOption::FindDirectChildrenOnly)) {
		qObject.push_back({ entity,nullptr });
	}
	while (!qObject.isEmpty()) {
		QPair<QEntity*, QTreeWidgetItem*> parent = qObject.takeFirst();
		if (parent.first) {
			QTreeWidgetItem* item = new QTreeWidgetItem({ parent.first->objectName() });
			parent.first->disconnect();

			item->setData(1, 0, QVariant::fromValue(parent.first));
			if (parent.second == nullptr) {
				addTopLevelItem(item);
			}
			else {
				parent.second->addChild(item);
			}
			parent.first->disconnect(this);
			connect(parent.first, &QObject::objectNameChanged, this, [item](const QString& name) {
				item->setText(0, name);
			});
			for (auto child : parent.first->findChildren<QEntity*>(QString(), Qt::FindChildOption::FindDirectChildrenOnly)) {
				qObject.push_back({ child,item });
			}
		}
	}
	expandAll();
}

void QScenePanel::showEvent(QShowEvent* event) {
	updateUI();
}

void QScenePanel::mousePressEvent(QMouseEvent* event) {
	mClickPos = event->pos();
	QTreeWidget::mousePressEvent(event);
}

void QScenePanel::mouseReleaseEvent(QMouseEvent* event) {
	if (mClickPos == event->pos()&&event->button()==Qt::RightButton) {
		QMenu menu;
		menu.addAction("Create Entity", [this]() {
			mWorld->createEntity();
		});
		menu.exec(QCursor::pos());
	}
	mClickPos = { 0,0 };
	QTreeWidget::mouseReleaseEvent(event);
}
