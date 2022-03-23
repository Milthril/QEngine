#include "FileListWidget.h"
#include "QStyledItemDelegate"
#include "QPainter"
#include <QQueue>
#include <QtConcurrent/QtConcurrent>
#include <QScrollBar>

const QSize GridSize(80, 100);

class FileListItemDelegate :public QStyledItemDelegate {
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		if (!index.isValid())
			return;
		painter->save();
		QString text = index.data(Qt::DisplayRole).toString();
		QRect expendRect(option.rect.x(), option.rect.y(), GridSize.width(), GridSize.height());

		if (option.state & QStyle::State_Selected) {
			painter->fillRect(expendRect, QColor(1, 100, 200, 150));
		}
		if (option.state & QStyle::State_MouseOver) {
			painter->fillRect(expendRect, QColor(1, 100, 200, 50));
		}

		QIcon icon;
		QFileInfo fileInfo(index.data(Qt::ToolTipRole).toString());
		if (fileInfo.isDir()) {
			icon = dirType;
		}
		else {
			if (fileInfo.suffix() == "png" || fileInfo.suffix() == "jpg" || fileInfo.suffix() == "jepg")
				icon = cacheTask_->getIcon(fileInfo.filePath());
			if (icon.isNull())
				icon = fileType;
		}
		icon.paint(painter, QRect(expendRect.x(), expendRect.y() + 5, GridSize.width(), option.decorationSize.height()), Qt::AlignCenter, QIcon::Normal, option.state & QStyle::State_Open ? QIcon::On : QIcon::Off);

		painter->setBrush(option.palette.windowText());
		painter->setFont(option.font);
		painter->drawText(QRect(expendRect.x(), expendRect.y() + option.decorationSize.height(), GridSize.width(), GridSize.height() - option.decorationSize.height()), Qt::AlignCenter | Qt::TextWrapAnywhere, text);
		painter->restore();
	}

	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
		return GridSize;
	}
public:
	QIcon fileType = QIcon(":/Resources/Icons/24gf-fileEmpty.png");
	QIcon dirType = QIcon(":/Resources/Icons/24gf-folderMinus.png");
	FileListWidegtThreadTask* cacheTask_;
};

FileListWidget::FileListWidget() :threadTask_(this) {
	setMovement(QListView::Static);
	setFrameShape(QFrame::NoFrame);
	setViewMode(QListView::IconMode);
	setUniformItemSizes(true);
	FileListItemDelegate* itemDelegate = new FileListItemDelegate;
	itemDelegate->cacheTask_ = &threadTask_;
	setItemDelegate(itemDelegate);
	setItemAlignment(Qt::AlignCenter);
	setIconSize(QSize(40, 40));
	setGridSize(GridSize);
	setResizeMode(QListView::Adjust);
	setTextElideMode(Qt::TextElideMode::ElideNone);
	setAttribute(Qt::WA_NativeWindow, true);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int position) {
		threadTask_.clearIconCache();
	});
}

FileListWidget::~FileListWidget()
{
}

void FileListWidget::setCurrentDir(QString dir) {
	currentDir_ = dir;
	threadTask_.updateFileItems();
}

void FileListWidget::setCurrentDirAndSearch(QString dir, QString keyword)
{
	currentDir_ = dir;
	if (keyword.isEmpty()) {
		threadTask_.updateFileItems();
	}
	else {
		searchFile(keyword);
	}
}

void FileListWidget::addElideItem()
{
	QListWidgetItem* item = new QListWidgetItem("...");
	item->setFlags(Qt::ItemFlag::NoItemFlags);
	addItem(item);
}

void FileListWidget::searchFile(const QString& keyword)
{
	threadTask_.search(keyword);
}

void FileListWidget::submitItemCount()
{
	Q_EMIT newMessage(QString("items: %1").arg(count()));
}

FileListWidegtThreadTask::FileListWidegtThreadTask(FileListWidget* widget)
	:widget_(widget)
{
	thread_ = new QThread;
	moveToThread(thread_);
}

FileListWidegtThreadTask::~FileListWidegtThreadTask() {
	if (thread_->isRunning()) {
		thread_->requestInterruption();
		thread_->quit();
		thread_->wait();
	}
}

void FileListWidegtThreadTask::searchInternal(const QString& keyword) {
	QQueue<QDir> queue;
	queue.enqueue(widget_->currentDir_);
	while (!queue.isEmpty() && !thread_->isInterruptionRequested()) {
		QDir dir = queue.dequeue();
		auto entryList = dir.entryInfoList(QDir::Filter::Files | QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);
		for (int i = 0; i < entryList.size() && !thread_->isInterruptionRequested(); i++) {
			const QFileInfo& subFile = entryList[i];
			if (subFile.isDir()) {
				queue.enqueue(subFile.filePath());
			}
			else if (subFile.isFile() && subFile.fileName().contains(keyword)) {
				addCacheFile(subFile);
				if (widget_->count() + cacheList_.size() >= searchLimited) {
					queue.clear();
					submitCache();
					Q_EMIT widget_->newMessage(QString("items: %1+").arg(widget_->count()));
					widget_->addElideItem();
					break;
				}
			}
		}
	}
	submitCache();

	thread_->quit();
}

void FileListWidegtThreadTask::updateFileItemInternal()
{
	auto entryList = widget_->currentDir_.entryInfoList(QDir::Filter::Files | QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot, QDir::SortFlag::LocaleAware | QDir::SortFlag::DirsFirst);
	QStringList itemList;
	for (int i = 0; i < entryList.size() && !thread_->isInterruptionRequested(); i++) {
		itemList.push_back(entryList[i].fileName());
	}
	if (thread_->isInterruptionRequested())
		return;
	widget_->addItems(itemList);
	widget_->submitItemCount();
	for (int i = 0; i < entryList.size() && !thread_->isInterruptionRequested(); i++) {
		QListWidgetItem* item = widget_->item(i);
		if (item != nullptr) {
			item->setData(Qt::ToolTipRole, entryList[i].filePath());
			//item->setIcon(widget_->getIcon(entryList[i]));
		}
	}
	thread_->quit();
}

void FileListWidegtThreadTask::cacheIcon(const QString filePath)
{
	if (iconCacheMap_.contains(filePath))
		return;
	iconCacheMap_[filePath] = QIcon(filePath);
	thread_->quit();
	widget_->update();
}

void FileListWidegtThreadTask::addCacheFile(const QFileInfo& info) {
	cacheList_ << info;
	if (cacheList_.size() > cacheSize) {
		submitCache();
		widget_->submitItemCount();
	}
}

void FileListWidegtThreadTask::submitCache()
{
	if (cacheList_.isEmpty())
		return;
	QStringList nameList;
	for (auto& info : cacheList_) {
		nameList << info.fileName();
	}
	if (thread_->isInterruptionRequested())
		return;
	int offset = widget_->count();
	widget_->addItems(nameList);
	if (thread_->isInterruptionRequested())
		return;
	for (int i = 0; i < cacheList_.size(); i++) {
		QListWidgetItem* item = widget_->item(offset + i);
		if (item != nullptr) {
			item->setData(Qt::ToolTipRole, cacheList_[i].filePath());
			//item->setIcon(widget_->getIcon(cacheList_[i]));
		}
	}
	cacheList_.clear();
}

void FileListWidegtThreadTask::search(const QString& keyword)
{
	if (thread_->isRunning()) {
		QEventLoop eventloop;
		connect(thread_, &QThread::finished, &eventloop, &QEventLoop::quit);
		thread_->requestInterruption();
		if (!thread_->isFinished())
			eventloop.exec();
	}
	if (keyword.isEmpty()) {
		updateFileItems();
		return;
	}
	widget_->clear();
	thread_->start();
	QMetaObject::invokeMethod(this, "searchInternal", Q_ARG(QString, keyword));
}

void FileListWidegtThreadTask::updateFileItems()
{
	if (thread_->isRunning()) {
		QEventLoop eventloop;
		connect(thread_, &QThread::finished, &eventloop, &QEventLoop::quit);
		thread_->requestInterruption();
		if (!thread_->isFinished())
			eventloop.exec();
	}
	widget_->clear();
	thread_->start();
	QMetaObject::invokeMethod(this, "updateFileItemInternal");
}

void FileListWidegtThreadTask::clearIconCache()
{
	mutex.lock();
	iconCacheMap_.clear();
	mutex.unlock();
}

QIcon FileListWidegtThreadTask::getIcon(const QString& filePath)
{
	QIcon icon = iconCacheMap_.value(filePath);
	if (icon.isNull() && !widget_->verticalScrollBar()->isSliderDown()) {
		if (thread_->isRunning())
			thread_->quit();
		thread_->start();
		QMetaObject::invokeMethod(this, "cacheIcon", Q_ARG(QString, filePath));
	}
	return icon;
}