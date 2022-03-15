#include "SceneTreeWidget.h"
#include <memory>

SceneTreeWidget::SceneTreeWidget(std::shared_ptr<QScene> scene)
	:mScene(scene)
{
	createUI();
}

void SceneTreeWidget::createUI()
{
	setColumnCount(1);
	setIndentation(8);
	setHeaderHidden(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setColumnWidth(0, 120);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setFrameStyle(QFrame::NoFrame);

	mRoot = std::make_shared<QTreeWidgetItem>(QStringList("Root"));
	addTopLevelItem(mRoot.get());
}