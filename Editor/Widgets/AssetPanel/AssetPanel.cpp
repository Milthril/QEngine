#include "AssetPanel.h"
#include "QBoxLayout"
#include <QPainter>
#include <QApplication>
#include <QMenu>
#include <QSplitter>
#include <QAction>
#include <QActionGroup>
#include <QWidgetAction>
#include "Adjuster\BoolBox.h"
#include "Toolkit\QWidgetShadowMaker.h"
#include "Asset\Material.h"
#include "Utils\FileUtils.h"


AssetPanel::AssetPanel(QString rootDir)
	: rootDir_(rootDir)
	, directoryWidget_(rootDir)
	, pathViewer_(rootDir)
	, mFileFileterIcon(":/Resources/Icons/Filter.png")
{
	if (!rootDir_.exists()) {
		rootDir_ = QDir::home();
	}
	createUI();
	connectUI();
	fileWidget_.setCurrentDir(rootDir_.path());
	pathViewer_.setCurrentDir(rootDir_.path());
}

void AssetPanel::createUI()
{
	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->setContentsMargins(5, 5, 5, 5);
	vLayout->setSpacing(0);
	QSplitter* splitter = new QSplitter;

	QWidget* leftPanel = new QWidget;
	QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
	leftLayout->setContentsMargins(0, 0, 0, 0);
	leftLayout->addWidget(&directorySeacher_);
	leftLayout->addWidget(&directoryWidget_);

	btFileFilter_.setText("Filter");
	btFileFilter_.setFixedHeight(20);
	mFileFileterIcon.setUpdateCallBack([this]() {
		btFileFilter_.setIcon(mFileFileterIcon.getIcon());
	 });

	QWidget* rightPanel = new QWidget;
	QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
	rightLayout->setSpacing(5);
	rightLayout->setContentsMargins(0, 0, 0, 0);
	QHBoxLayout* h = new QHBoxLayout();
	h->addWidget(&btFileFilter_);
	h->addWidget(&fileSearcher_);
	rightLayout->addLayout(h);
	rightLayout->addWidget(&fileWidget_);
	rightLayout->addWidget(&fileStatusBar_);
	splitter->addWidget(leftPanel);
	splitter->addWidget(rightPanel);

	splitter->setStretchFactor(0, 2);
	splitter->setStretchFactor(1, 5);

	vLayout->addWidget(&pathViewer_);
	vLayout->addWidget(splitter);

	directorySeacher_.setPlaceholderText("search directory");
	directorySeacher_.setFrame(QFrame::NoFrame);
	fileSearcher_.setPlaceholderText("search file");
	fileSearcher_.setFrame(QFrame::NoFrame);

	fileSearcher_.setGraphicsEffect(new QWidgetShadowMaker);
	directorySeacher_.setGraphicsEffect(new QWidgetShadowMaker);
	btFileFilter_.setGraphicsEffect(new QWidgetShadowMaker);
}

void AssetPanel::connectUI(){
	connect(&fileWidget_, &QListWidget::doubleClicked, this, [this](QModelIndex index) {
		QFileInfo fileInfo(index.data(Qt::ToolTipRole).toString());
		if (fileInfo.isDir()) {
			directoryWidget_.setCurrentDir(fileInfo.filePath());
		}
		else {
			QFile file(index.data(Qt::ToolTipRole).toString());
			if(file.open(QFile::ReadOnly)) {
				QDataStream stream(&file);
				int typeId = -1;
				stream >> typeId;
				file.close();
				switch (typeId) {
				case QMetaTypeId2<Asset::Material>::qt_metatype_id():
					break;
				default:
					break;
				}
			}
		}
	});

	connect(&fileWidget_, &FileListWidget::newMessage, this, [this](QString message) {
		fileStatusBar_.showMessage(message);
	});

	connect(&directoryWidget_, &QTreeWidget::itemPressed, this, [this](QTreeWidgetItem* item) {
		if (qApp->mouseButtons() & Qt::RightButton) {
			QMenu menu;
			menu.addAction("Show In Folder", [item]() {
				FileUtils::showInFolder(item->data(0, Qt::ToolTipRole).toString());
			});
			menu.exec(QCursor::pos());
		}
	});
	connect(&pathViewer_, &PathViewer::pathClicked, this, [this](QString path) {
		directoryWidget_.setCurrentDir(path);
	});

	connect(&directoryWidget_, &QTreeWidget::currentItemChanged, this, [this](QTreeWidgetItem* current, QTreeWidgetItem* previous) {
		QString dir = current->data(0, Qt::ToolTipRole).toString();
		fileWidget_.setCurrentDirAndSearch(dir, fileSearcher_.text());
		pathViewer_.setCurrentDir(dir);
		fileSearcher_.setPlaceholderText(QString("search file in [%1]").arg(QFileInfo(dir).fileName()));
	});

	connect(&fileSearcher_, &QLineEdit::textChanged, &fileWidget_, &FileListWidget::searchFile);
	connect(&directorySeacher_, &QLineEdit::textChanged, &directoryWidget_, &DirTreeWidget::searchDir);

	connect(&btFileFilter_, &QPushButton::clicked, this, [this]() {
		fileFliterMenu_.exec(btFileFilter_.mapToGlobal(btFileFilter_.rect().topRight()));
	});
}