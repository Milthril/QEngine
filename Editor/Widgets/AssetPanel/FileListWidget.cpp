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
#include "Toolkit/DropInterface.h"

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
			if (mIconList.contains(fileInfo.suffix())) {
				icon = mIconList[fileInfo.suffix()]->getIcon();
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
	QMap<QString,std::shared_ptr<QSvgIcon>> mIconList = {
		{IAsset::AssetExtName[IAsset::Material],std::make_shared<QSvgIcon>(":/Resources/Icons/Material.png")},
		{IAsset::AssetExtName[IAsset::SkyBox],std::make_shared<QSvgIcon>(":/Resources/Icons/SkyBox.png")},
		{IAsset::AssetExtName[IAsset::StaticMesh],std::make_shared<QSvgIcon>(":/Resources/Icons/StaticMesh.png")},
		{IAsset::AssetExtName[IAsset::Skeleton],std::make_shared<QSvgIcon>(":/Resources/Icons/Skeleton.png")},
		{IAsset::AssetExtName[IAsset::SkeletonAnimation],std::make_shared<QSvgIcon>(":/Resources/Icons/SkeletonAnimation.png")},
		{IAsset::AssetExtName[IAsset::SkeletonModel],std::make_shared<QSvgIcon>(":/Resources/Icons/SkeletonModel.png")},
		{IAsset::AssetExtName[IAsset::ParticleSystem],std::make_shared<QSvgIcon>(":/Resources/Icons/ParticleSystem.png")},
	};
	QSvgIcon mDirIcon = QSvgIcon(":/Resources/Icons/24gf-folder4.png");
	QSvgIcon mFileIcon = QSvgIcon(":/Resources/Icons/24gf-fileEmpty.png");

};


FileListWidget::FileListWidget()  {
	setMovement(QListView::Static);
	setFrameShape(QFrame::NoFrame);
	setViewMode(QListView::IconMode);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setUniformItemSizes(true);
	FileListItemDelegate* itemDelegate = new FileListItemDelegate;
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
			menu.addAction("Remove", [path, this]() {
				const auto& items = this->selectedItems();
				for (auto& item : items) {
					QString path = item->data(Qt::ToolTipRole).toString();
					QFile file(path);
					file.remove();
				}
			});
			menu.exec(QCursor::pos());
		}
	});

	connect(this, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
		std::shared_ptr<IAsset> asset = TheAssetMgr->load(item->data(Qt::ToolTipRole).toString());
		if (asset && asset->type() == IAsset::Material) {
			QMaterialEditor* editor = new QMaterialEditor(std::dynamic_pointer_cast<Asset::Material>(asset));
			editor->show();
			editor->activateWindow();
			editor->setFocus();
		}
		else if (asset && asset->type() == IAsset::ParticleSystem) {
		QParticlesEditor* editor = new QParticlesEditor(std::dynamic_pointer_cast<Asset::ParticleSystem>(asset));
			editor->show();
			editor->activateWindow();
			editor->setFocus();
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

	connect(&fileWatcher_, &QFileSystemWatcher::directoryChanged, this, [this](const QString& path) {
		updateCurrentItem();
	});
}

FileListWidget::~FileListWidget()
{
}

void FileListWidget::setCurrentDir(QString dir) {
	fileWatcher_.removePath(currentDir_.path());
	currentDir_ = dir;
	updateCurrentItem();
	fileWatcher_.addPath(currentDir_.path());
}

void FileListWidget::setCurrentDirAndSearch(QString dir, QString keyword)
{
	fileWatcher_.removePath(currentDir_.path());
	currentDir_ = dir;
	fileWatcher_.addPath(currentDir_.path());
	if (keyword.isEmpty()) {
		updateCurrentItem();
	}
	else {
		searchFile(keyword);
	}
}

void FileListWidget::updateCurrentItem() {
	clear();
	auto entryList =currentDir_.entryInfoList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot, QDir::SortFlag::LocaleAware);
	QStringList nameFilerList;
	for (auto extName : IAsset::AssetExtName.values()) {
		nameFilerList << "*." + extName;
	}
	entryList += currentDir_.entryInfoList(nameFilerList, QDir::Filter::Files, QDir::SortFlag::Type);
	QStringList itemList;
	for (int i = 0; i < entryList.size() ; i++) {
		itemList.push_back(entryList[i].baseName());
	}
	addItems(itemList);
	submitItemCount();
	for (int i = 0; i < entryList.size(); i++) {
		QListWidgetItem* item = this->item(i);
		if (item != nullptr) {
			item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
			item->setData(Qt::ToolTipRole, entryList[i].filePath());
		}
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
	QQueue<QDir> queue;
	queue.enqueue(currentDir_);
	QStringList pathList;
	while (!queue.isEmpty()) {
		QDir dir = queue.dequeue();
		auto entryList = dir.entryInfoList(QDir::Filter::Files | QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);
		for (int i = 0; i < entryList.size(); i++) {
			const QFileInfo& subFile = entryList[i];
			if (subFile.isDir()) {
				queue.enqueue(subFile.filePath());
			}
			else if (subFile.isFile() && subFile.fileName().contains(keyword)) {
				pathList << subFile.path();
			}
		}
	}

	for (auto& path : pathList) {
		QListWidgetItem* item = new QListWidgetItem;
		item->setText(QFileInfo(path).baseName());
		if (item != nullptr) {
			item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
			item->setData(Qt::ToolTipRole, path);
		}
	}
}

void FileListWidget::submitItemCount()
{
	Q_EMIT newMessage(QString("items: %1").arg(count()));
}

void FileListWidget::dragEnterEvent(QDragEnterEvent* event) {
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
	DropInterface::startDrag(event->mimeData());
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

void FileListWidget::startDrag(Qt::DropActions supportedActions) {
	QListWidget::startDrag(supportedActions);
	DropInterface::endDrag();
}
