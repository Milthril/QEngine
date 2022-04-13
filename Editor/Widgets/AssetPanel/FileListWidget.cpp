#include "FileListWidget.h"
#include "QStyledItemDelegate"
#include "QPainter"
#include <QQueue>
#include <QtConcurrent/QtConcurrent>
#include <QScrollBar>
#include "Toolkit/QSvgIcon.h"
#include "QDragEnterEvent"
#include "Asset/Importer/QAssetImporter.h"

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
			icon = mDirIcon.getIcon();
		}
		else {
			if (fileInfo.suffix() == "png" || fileInfo.suffix() == "jpg" || fileInfo.suffix() == "jepg")
				icon = cacheTask_->getIcon(fileInfo.filePath());
			if (icon.isNull())
				icon = mFileIcon.getIcon();
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
	QSvgIcon mFileIcon = QSvgIcon(":/Resources/Icons/24gf-fileEmpty.png");
	QSvgIcon mDirIcon = QSvgIcon(":/Resources/Icons/24gf-folder4.png");
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
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
	setDefaultDropAction(Qt::MoveAction);
	this->setDragDropOverwriteMode(true);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int position) {
		threadTask_.clearIconCache();
	});
	connect(&fileWatcher_, &QFileSystemWatcher::directoryChanged, this, [this](const QString& path) {
		threadTask_.updateFileItems();
	});
}

FileListWidget::~FileListWidget()
{
}

void FileListWidget::setCurrentDir(QString dir) {
	fileWatcher_.removePath(currentDir_.path());
	currentDir_ = dir;
	threadTask_.updateFileItems();
	fileWatcher_.addPath(currentDir_.path());
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

void FileListWidget::dragEnterEvent(QDragEnterEvent* event) {
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
}

void FileListWidget::dragMoveEvent(QDragMoveEvent* event) {
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
}

void FileListWidget::dropEvent(QDropEvent* event) {
	QList <QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty()) {
		return;
	}
	for (auto& url : urls) {
		QAssetImpoerter::instance()->import(url.toLocalFile(),currentDir_);
	}
	QListWidget::dropEvent(event);
}


FileListWidegtThreadTask::FileListWidegtThreadTask(FileListWidget* widget)
	:widget_(widget)
{
	thread_ = new QThread;
	moveToThread(thread_);
	thread_->start();
}

FileListWidegtThreadTask::~FileListWidegtThreadTask() {
	if (thread_->isRunning()) {
		thread_->quit();
		thread_->wait();
	}
}

void FileListWidegtThreadTask::searchInternal(const QString& keyword) {
	onActive();
	widget_->clear();
	QQueue<QDir> queue;
	queue.enqueue(widget_->currentDir_);
	while (!queue.isEmpty() && !bRequestQuit) {
		QDir dir = queue.dequeue();
		auto entryList = dir.entryInfoList(QDir::Filter::Files | QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);
		for (int i = 0; i < entryList.size() && !bRequestQuit; i++) {
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
	onFinished();
}

void FileListWidegtThreadTask::updateFileItemInternal()
{
	onActive();
	widget_->clear();
	auto entryList = widget_->currentDir_.entryInfoList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot, QDir::SortFlag::LocaleAware);
	entryList += widget_->currentDir_.entryInfoList({ "*.QAsset" }, QDir::Filter::Files, QDir::SortFlag::LocaleAware);
	QStringList itemList;
	for (int i = 0; i < entryList.size() && !bRequestQuit; i++) {
		itemList.push_back(entryList[i].fileName());
	}
	if (bRequestQuit)
		return;
	widget_->addItems(itemList);
	widget_->submitItemCount();
	for (int i = 0; i < entryList.size() && !bRequestQuit; i++) {
		QListWidgetItem* item = widget_->item(i);
		if (item != nullptr) {
			item->setData(Qt::ToolTipRole, entryList[i].filePath());
		}
	}
	onFinished();
}

void FileListWidegtThreadTask::cacheIcon(const QString filePath)
{
	onActive();
	if (iconCacheMap_.contains(filePath))
		return;
	iconCacheMap_[filePath] = QIcon(filePath);
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
	if (bRequestQuit)
		return;
	int offset = widget_->count();
	widget_->addItems(nameList);
	if (bRequestQuit)
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

void FileListWidegtThreadTask::waitFinished() {
	if (bIsRunning) {
		bRequestQuit = true;
		//std::future<bool> future = bFinished.get_future();
		//future.wait();
	}
}

void FileListWidegtThreadTask::onActive() {
	bIsRunning = true;
	bRequestQuit = false;
	bFinished = std::promise<bool>();
}

void FileListWidegtThreadTask::onFinished() {
	bFinished.set_value(true);
	bIsRunning = false;
}


void FileListWidegtThreadTask::search(const QString& keyword)
{
	if (keyword.isEmpty()) {
		updateFileItems();
		return;
	}
	waitFinished();
	QMetaObject::invokeMethod(this, "searchInternal", Q_ARG(QString, keyword));
}

void FileListWidegtThreadTask::updateFileItems()
{
	waitFinished();
	QMetaObject::invokeMethod(this, &FileListWidegtThreadTask::updateFileItemInternal);
}

void FileListWidegtThreadTask::clearIconCache()
{
	iconCacheMap_.clear();
}

QIcon FileListWidegtThreadTask::getIcon(const QString& filePath)
{
	QIcon icon = iconCacheMap_.value(filePath);
	if (icon.isNull() && !widget_->verticalScrollBar()->isSliderDown()) {
		waitFinished();
		QMetaObject::invokeMethod(this, "cacheIcon", Q_ARG(QString, filePath));
	}
	return icon;
}