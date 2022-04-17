#include "FileListWidget.h"
#include "Asset/Importer/QAssetImporter.h"
#include "Asset/Material.h"
#include "Asset/PartcleSystem/ParticleSystem.h"
#include "QDragEnterEvent"
#include "QMenu"
#include "QPainter"
#include "QStyledItemDelegate"
#include "Serialization/QSerialization.h"
#include "Toolkit/QSvgIcon.h"
#include <QQueue>
#include <QScrollBar>
#include <QtConcurrent/QtConcurrent>
#include <QApplication>
#include "Window/MaterialEditor/QMaterialEditor.h"
#include "Window/ParticlesEditor/QParticlesEditor.h"
#include "Utils\FileUtils.h"
#include "Asset/GAssetMgr.h"

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
			else if (fileInfo.suffix() == IAsset::AssetExtName[IAsset::Material]) {
				icon = mMaterialIcon.getIcon();
			}
			else if (fileInfo.suffix() == IAsset::AssetExtName[IAsset::SkyBox]) {
				icon = mSkyBoxIcon.getIcon();;
			}
			else if (fileInfo.suffix() == IAsset::AssetExtName[IAsset::StaticMesh]) {
				icon = mStaticMeshIcon.getIcon();;
			}
			else if (icon.isNull())
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
	QSvgIcon mStaticMeshIcon = QSvgIcon(":/Resources/Icons/StaticMesh.png");
	QSvgIcon mSkyBoxIcon = QSvgIcon(":/Resources/Icons/SkyBox.png");
	QSvgIcon mMaterialIcon = QSvgIcon(":/Resources/Icons/Material.png");
	FileTaskThread* cacheTask_;
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
	setDragDropOverwriteMode(true);
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, &QListWidget::itemPressed, this, [this](QListWidgetItem* item) {
		if (qApp->mouseButtons() & Qt::RightButton) {
			QString path = item->data(Qt::ToolTipRole).toString();
			QMenu menu;
			menu.addAction("Rename", [this,item]() {
				this->editItem(item);
			});

			menu.addAction("Show In Folder", [path]() {
				FileUtils::showInFolder(path);
			});
			if (QFileInfo(path).isFile()) {
				menu.addAction("Remove", [path, this]() {
					QFile file(path);
					file.remove();
					threadTask_.updateFileItems();
			});
			}
			menu.exec(QCursor::pos());
		}
	});

	connect(this, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
		std::shared_ptr<IAsset> asset = TheAssetMgr->load(item->data(Qt::ToolTipRole).toString());
		if (asset && asset->type() == IAsset::Material) {
			QMaterialEditor::instance()->edit(std::dynamic_pointer_cast<Asset::Material>(asset));
		}
		else if (asset && asset->type() == IAsset::ParticleSystem) {
			QParticlesEditor::instance()->edit(std::dynamic_pointer_cast<Asset::ParticleSystem>(asset));
		}
	});


	connect(this, &QWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
		QMenu menu;
		menu.addSeparator();
		menu.addAction("New Material", [this]() {
			TheAssetMgr->createNewAsset(currentDir_, IAsset::Material);
		});
		menu.addAction("New ParticleSystem", [this]() {
			TheAssetMgr->createNewAsset(currentDir_, IAsset::ParticleSystem);
		});
		menu.exec(mapToGlobal(pos));
	});

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
	fileWatcher_.removePath(currentDir_.path());
	currentDir_ = dir;
	fileWatcher_.addPath(currentDir_.path());
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
		TheAssetMgr->import(url.toLocalFile(),currentDir_);
	}
	QListWidget::dropEvent(event);
}


FileTaskThread::FileTaskThread(FileListWidget* widget)
	:widget_(widget)
{
	thread_ = new QThread;
	moveToThread(thread_);
	thread_->start();
}

FileTaskThread::~FileTaskThread() {
	if (thread_->isRunning()) {
		thread_->quit();
		thread_->wait();
	}
}

void FileTaskThread::searchThreadTask(const QString& keyword) {
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

void FileTaskThread::updateWidgetTaskThread()
{
	onActive();
	widget_->clear();
	auto entryList = widget_->currentDir_.entryInfoList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot, QDir::SortFlag::LocaleAware);

	QStringList nameFilerList;
	for (auto extName : IAsset::AssetExtName.values()) {
		nameFilerList << "*." + extName;
	}
	entryList += widget_->currentDir_.entryInfoList(nameFilerList, QDir::Filter::Files, QDir::SortFlag::LocaleAware);
	QStringList itemList;
	for (int i = 0; i < entryList.size() && !bRequestQuit; i++) {
		itemList.push_back(entryList[i].baseName());
	}
	if (bRequestQuit)
		return;
	widget_->addItems(itemList);
	widget_->submitItemCount();
	for (int i = 0; i < entryList.size() && !bRequestQuit; i++) {
		QListWidgetItem* item = widget_->item(i);
		if (item != nullptr) {
			item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
			item->setData(Qt::ToolTipRole, entryList[i].filePath());
		}
	}
	onFinished();
}

void FileTaskThread::updateIconTaskThread(const QString filePath)
{
	onActive();
	if (iconCacheMap_.contains(filePath))
		return;
	iconCacheMap_[filePath] = QIcon(filePath);
	widget_->update();
}

void FileTaskThread::addCacheFile(const QFileInfo& info) {
	cacheList_ << info;
	if (cacheList_.size() > cacheSize) {
		submitCache();
		widget_->submitItemCount();
	}
}

void FileTaskThread::submitCache()
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

void FileTaskThread::waitFinished() {
	if (bIsRunning) {
		bRequestQuit = true;
		//std::future<bool> future = bFinished.get_future();
		//future.wait();
	}
}

void FileTaskThread::onActive() {
	bIsRunning = true;
	bRequestQuit = false;
	bFinished = std::promise<bool>();
}

void FileTaskThread::onFinished() {
	bFinished.set_value(true);
	bIsRunning = false;
}


void FileTaskThread::search(const QString& keyword)
{
	if (keyword.isEmpty()) {
		updateFileItems();
		return;
	}
	waitFinished();
	QMetaObject::invokeMethod(this, "searchThreadTask", Q_ARG(QString, keyword));
}

void FileTaskThread::updateFileItems()
{
	waitFinished();
	QMetaObject::invokeMethod(this, &FileTaskThread::updateWidgetTaskThread);
}

void FileTaskThread::clearIconCache()
{
	iconCacheMap_.clear();
}

QIcon FileTaskThread::getIcon(const QString& filePath)
{
	QIcon icon = iconCacheMap_.value(filePath);
	if (icon.isNull() && !widget_->verticalScrollBar()->isSliderDown()) {
		waitFinished();
		QMetaObject::invokeMethod(this, "updateIconTaskThread", Q_ARG(QString, filePath));
	}
	return icon;
}