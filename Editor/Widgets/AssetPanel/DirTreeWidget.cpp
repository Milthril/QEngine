#include "DirTreeWidget.h"
#include "QStyledItemDelegate"
#include "QPainter"
//class DireTreeItemDelegate :public QStyledItemDelegate {
//protected:
//	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
//		if (!index.isValid())
//			return;
//		QString text = index.data(Qt::DisplayRole).toString();
//		painter->save();
//
//		qDebug() << text << option.palette << option.backgroundBrush;
//		if (option.state & QStyle::State_Selected) {
//			painter->fillRect(option.rect, option.palette.brush(QPalette::Window));
//			painter->setPen(Qt::white);
//		}
//
//		if (option.state & QStyle::State_MouseOver) {
//			painter->fillRect(option.rect, QColor(100, 100, 100, 100));
//		}
//
//		QRect iconRect = option.rect;
//		iconRect.setWidth(iconRect.height());
//		iconRect.adjust(3, 3, -3, -3);
//		if (option.state & QStyle::State_Open)
//			painter->drawPixmap(iconRect, QPixmap(":/Icons/Resources/Icons/24gf-folderOpen.png"));
//		else
//			painter->drawPixmap(iconRect, QPixmap(":/Icons/Resources/Icons/24gf-folderMinus.png"));
//		QRect textRect = option.rect;
//		textRect.setLeft(textRect.left() + textRect.height());
//		painter->drawText(textRect, Qt::AlignLeft, text);
//		painter->restore();
//	}
//
//	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
//		return QStyledItemDelegate::sizeHint(option, index);
//	}
//};

DirTreeWidget::DirTreeWidget(QString rootDir)
	:rootDir_(rootDir) {
	setHeaderHidden(true);
	setFrameShape(QFrame::NoFrame);
	setIconSize(QSize(16, 16));
	setIndentation(10);
	intiDirectories();
	//setItemDelegate(new DireTreeItemDelegate);
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
			itemMap_[subDir.filePath()] = subItem;
			subItem->setData(0, Qt::ToolTipRole, subDir.filePath());
			item->addChild(subItem);
			processDir(subDir.filePath(), subItem);
		}
	}
}

void DirTreeWidget::intiDirectories() {
	for (auto& subDir : rootDir_.entryInfoList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot)) {
		if (subDir.isDir()) {
			QTreeWidgetItem* item = new QTreeWidgetItem({ subDir.fileName() });
			item->setData(0, Qt::ToolTipRole, subDir.filePath());
			itemMap_[subDir.filePath()] = item;
			addTopLevelItem(item);
			processDir(subDir.filePath(), item);
		}
	}
}