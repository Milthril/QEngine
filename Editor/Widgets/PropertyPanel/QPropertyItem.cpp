#include "QPropertyItem.h"
#include "QPropertyPanel.h"

QPropertyItem::QPropertyItem()
{
}

QPropertyItem::~QPropertyItem()
{
}

void QPropertyItem::updatePanel() {
	QPropertyPanel* panel = dynamic_cast<QPropertyPanel*>(this->treeWidget());
	if (panel) {
		panel->updatePanel();
	}
}

void QPropertyItem::recreatePanel() {
	QPropertyPanel* panel = dynamic_cast<QPropertyPanel*>(this->treeWidget());
	if (panel) {
		panel->recreatePanel();
	}
}
