#include "DirTreeWidget.h"
#include "QStyledItemDelegate"
#include "QPainter"
#include "Toolkit\QSvgIcon.h"
#include <QApplication>

class DireTreeItemDelegate :public QStyledItemDelegate {
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
		if (!index.isValid())
			return;
		QStyleOptionViewItem newOption(option);
		if (option.state & QStyle::State_Open)
			newOption.icon=mDirOpenIcon.getIcon();
		else
			newOption.icon=mDirCloseIcon.getIcon();
		const QWidget* widget = option.widget;
		QStyle* style = widget ? widget->style() : QApplication::style();
		style->drawControl(QStyle::CE_ItemViewItem, &newOption, painter, widget);
		QStyledItemDelegate::paint(painter, newOption, index);
	}

	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
		return QStyledItemDelegate::sizeHint(option, index);
	}
private:
	QSvgIcon mDirOpenIcon = QSvgIcon(":/Resources/Icons/24gf-folderOpen.png");
	QSvgIcon mDirCloseIcon = QSvgIcon(":/Resources/Icons/24gf-folder3.png");
};

DirTreeWidget::DirTreeWidget(QString rootDir)
	:rootDir_(rootDir) {
	setHeaderHidden(true);
	setFrameShape(QFrame::NoFrame);
	setIconSize(QSize(16, 16));
	setIndentation(10);
	intiDirectories();
	setItemDelegate(new DireTreeItemDelegate);
}

void DirTreeWidget::setCurrentDir(QString dir)
{
	auto ret = itemMap_.find(dir);
	if (ret != itemMap_.end()) {
		setCurrentItem(*ret);
	}
}

void DirTreeWidget::searchDir(QString keyword)
{
	if (keyword.isEmpty()) {
		for (auto& item : itemMap_) {
			item->setHidden(false);
		}
		return;
	}
	QList<QTreeWidgetItem*> itemList;
	for (auto& item : itemMap_) {
		item->setHidden(true);
		if (item->data(0, Qt::ToolTipRole).toString().contains(keyword)) {
			itemList << item;
		}
	}
	for (QTreeWidgetItem* curItem : itemList) {
		while (curItem != nullptr && curItem->isHidden()) {
			curItem->setHidden(false);
			curItem = curItem->parent();
			this->expandItem(curItem);
		}
	}
}

void DirTreeWidget::processDir(QDir dir, QTreeWidgetItem* item) {
	for (auto& subDir : dir.entryInfoList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot)) {
		if (subDir.isDir()) {
			QTreeWidgetItem* subItem = new QTreeWidgetItem({ subDir.fileName() });
			subItem->setBackground(0, Qt::transparent);
			itemMap_[subDir.filePath()] = subItem;
			subItem->setData(0, Qt::ToolTipRole, subDir.filePath());
			item->addChild(subItem);
			processDir(subDir.filePath(), subItem);
		}
	}
}

void DirTreeWidget::intiDirectories() {
	QTreeWidgetItem* item = new QTreeWidgetItem({ rootDir_.dirName() });
	itemMap_[rootDir_.path()] = item;
	item->setBackground(0, Qt::transparent);
	item->setData(0, Qt::ToolTipRole, rootDir_.path());
	addTopLevelItem(item);
	processDir(rootDir_, item);
}