#include "EditorAttributeMgr.h"
#include <QInputDialog>
#include <QActionGroup>
#include <QApplication>
#include "Widgets\Buttons\WinCloseButton.h"
#include "Widgets\Buttons\WinMinButton.h"
#include "Widgets\Buttons\WinMaxButton.h"
#include "QJsonDocument"
#include "QFontDatabase"
#include "kddockwidgets\private\widgets\FloatingWindowWidget_p.h"
#include "QAeroWindowMaker.h"

EditorAttributeMgr::EditorAttributeMgr()
{
}

void EditorAttributeMgr::initConfig()
{
	initDefaultAttribute();
	loadCurrentCtx();
	loadAllStyleSheet();
	loadFont();
}

QDir EditorAttributeMgr::getLayoutDir() {
	return mSaveDir.filePath("Layout");
}

QDir EditorAttributeMgr::getStyleSheetDir() {
	return mSaveDir.filePath("StyleSheet");
}

void EditorAttributeMgr::initDefaultAttribute() {
	if (mSaveDir.exists())
		return;
	mSaveDir.mkpath(getLayoutDir().path());
	mSaveDir.mkpath(getStyleSheetDir().path());;

	writeFile(getStyleSheetDir().filePath("Dark.qss"), R"(

QWidget{
	font-size: 10pt;
	background-color:rgb(45,45,48);
	color:white;
}
CodeSearchEditor{
	background-color:rgb(10,10,10);
	border-radius: 3px;
}
QTreeWidget,QListWidget{
	background-color: rgb(30,30,30);
}
QLineEdit{
	background-color:rgb(51,51,55);
	border-radius: 3px;
	color: rgba(255,255,255,150);
	border: 1px inset transparent;
}
QLineEdit:hover{
	background-color:rgb(63,63,70);
	border: 1px inset rgba(0,120,215,150);
}

QPushButton{
    padding:  0px 5px 0px 5px;
    border-radius: 0px;
    border: 1px solid rgba(200,200,200,200);
    background-color: rgba(200,200,200,100);
}

QPushButton:hover{
	background-color: rgb(28,151,234);
}

QTreeWidget::item:hover {
	background-color: rgba(0,120,215,150);
}

QTreeWidget::item:selected {
	background-color: rgb(0,120,215);
}

QScrollBar:vertical {
    width: 8px;
    background: #f0f0f0;
}
QScrollBar:horizontal {
    height: 8px;
    background: #f0f0f0;
}
QScrollBar::handle:vertical {
    background: #cdcdcd;
    min-height: 30px;
}
QScrollBar::handle:horizontal {
    background: #cdcdcd;
    min-width: 30px;
}
QScrollBar::handle:vertical:hover,
QScrollBar::handle:horizontal:hover {
    background: #a6a6a6;
}
QScrollBar::sub-line:vertical, QScrollBar::add-line:vertical,
QScrollBar::sub-line:horizontal, QScrollBar::add-line:horizontal {
    width: 0;
    height: 0;
}

QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical,
QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
    background: none;
}

QMenu{
	background-color: rgb(27,27,28);
}
QMenu::item:selected,QMenuBar::item:selected {
	background-color: rgb(62,62,64);
}
)");

	writeFile(getStyleSheetDir().filePath("Light.qss"), R"(
QWidget{
	font-size: 10pt;
	background-color:rgb(238,238,242);
	color:black;
}
CodeSearchEditor{
	background-color:rgb(204,213,240);
	border-radius: 3px;
}
QTreeWidget,QListWidget{
	background-color: white;
}
QLineEdit{
	background-color:white;
	border-radius: 3px;
	color:rgba(0,0,0,150);
	border: 1px inset transparent;
}
QLineEdit:hover{
	border: 1px inset rgba(0,120,215,150);
}

QTreeWidget::item:hover {
	background-color: rgb(201,222,245);
}
QTreeWidget::item:selected {
	background-color: rgb(0,120,215);
}
QScrollBar:vertical {
    width: 8px;
    background: #f0f0f0;
}
QScrollBar:horizontal {
    height: 8px;
    background: #f0f0f0;
}
QScrollBar::handle:vertical {
    background: #cdcdcd;
    min-height: 30px;
}
QScrollBar::handle:horizontal {
    background: #cdcdcd;
    min-width: 30px;
}
QScrollBar::handle:vertical:hover,
QScrollBar::handle:horizontal:hover {
    background: #a6a6a6;
}
QScrollBar::sub-line:vertical, QScrollBar::add-line:vertical,
QScrollBar::sub-line:horizontal, QScrollBar::add-line:horizontal {
    width: 0;
    height: 0;
}
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical,
QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
    background: none;
}

QPushButton{
    padding:  0px 5px 0px 5px;
    border-radius: 0px;
    border: 1px solid rgba(200,200,200,200);
    background-color: rgba(200,200,200,100);
}

QPushButton:pressed{
	background-color: rgb(0,122,204);
}
QPushButton:hover{
	background-color: rgb(201,222,245);
}
QMenu{
	background-color: white;
}
QMenu::item {
	height: 20px;
	padding: 0px 20px 0px 5px;
	margin: 2px
}
QMenu::item:selected,QMenuBar::item:selected {
	background-color: rgb(201,222,245);
}

)");

	loadStyleSheet(getStyleSheetDir().filePath("Light.qss"));
}

void EditorAttributeMgr::writeFile(QString filePath, QByteArray data)
{
	QFile file(filePath);
	if (file.open(QFile::WriteOnly)) {
		file.write(data);
		file.close();
	}
}

void EditorAttributeMgr::loadAllLayout()
{
	mLayoutMenu->clear();
	for (auto layoutFileInfo : getLayoutDir().entryInfoList(QDir::Filter::Files)) {
		auto it = layoutFileInfo.suffix();
		if (layoutFileInfo.suffix().compare("json", Qt::CaseInsensitive) == 0) {
			mLayoutActionMap[layoutFileInfo.baseName()] = mLayoutMenu->addAction(layoutFileInfo.baseName(), [this, layoutFileInfo]() {
				mSaver.restoreFromFile(layoutFileInfo.filePath());
				mCurrentCtx["Layout"] = layoutFileInfo.baseName();
				saveCurrentCtx();
			});
			QString current = mCurrentCtx["Layout"].toString();
			if (current == layoutFileInfo.baseName()) {
				mLayoutMenu->setActiveAction(mLayoutActionMap[layoutFileInfo.baseName()]);
				mLayoutActionMap[layoutFileInfo.baseName()]->setChecked(true);
			}
		};
	}
}

void EditorAttributeMgr::loadAllStyleSheet()
{
	mWindowStyleMenu->clear();
	QActionGroup* acStyleGroup = new QActionGroup(mWindowStyleMenu);
	for (auto styleFileInfo : getStyleSheetDir().entryInfoList(QDir::Filter::Files)) {
		auto it = styleFileInfo.suffix();
		if (styleFileInfo.suffix().compare("qss", Qt::CaseInsensitive) == 0) {
			QAction* action = mWindowStyleMenu->addAction(styleFileInfo.baseName(), [styleFileInfo, this]() {
				loadStyleSheet(styleFileInfo.filePath());
				mCurrentCtx["StyleSheet"] = styleFileInfo.baseName();
				saveCurrentCtx();
			});
			action->setCheckable(true);
			acStyleGroup->addAction(action);
			QString current = mCurrentCtx["StyleSheet"].toString();
			if (current == styleFileInfo.baseName()) {
				action->toggle();
				loadStyleSheet(styleFileInfo.filePath());
			}
		};
	}
}

void EditorAttributeMgr::loadStyleSheet(QString filePath)
{
	QFile file(filePath);
	if (file.open(QFile::ReadOnly)) {
		qApp->setStyleSheet(file.readAll());
		file.close();
	}
}

void EditorAttributeMgr::loadFont() {
	int id = QFontDatabase::addApplicationFont(":/Resources/Fonts/Barlow-Regular.ttf");
	QStringList fontList = QFontDatabase::applicationFontFamilies(id);
	QFont font(fontList.first());
	qApp->setFont(font);
}

void EditorAttributeMgr::saveCurrentCtx()
{
	writeFile(mSaveDir.filePath("Config.json"), QJsonDocument(mCurrentCtx).toJson());
}

void EditorAttributeMgr::loadCurrentCtx()
{
	QFile file(mSaveDir.filePath("Config.json"));
	if (file.open(QFile::ReadOnly)) {
		mCurrentCtx = QJsonDocument::fromJson(file.readAll()).object();
	}
}

void EditorAttributeMgr::saveCurrentLayout()
{
	bool ok;
	QString text = QInputDialog::getText(nullptr, QObject::tr("save window layout"),
										  QObject::tr("input the name of the layout:"), QLineEdit::Normal,
										 "New Window Layout", &ok);
	if (ok && !text.isEmpty()) {
		QString path = mSaveDir.filePath(text + ".json");

		mSaver.saveToFile(path);
		if (!mLayoutActionMap.contains(text)) {
			mLayoutActionMap[text] = mLayoutMenu->addAction(text, [this, text, path]() {
				mSaver.restoreFromFile(path);
				mCurrentCtx["Layout"] = text;
				saveCurrentCtx();
			});
		}
	}
}

void EditorAttributeMgr::tryCreateDefaultLayout() {
	if (!QFile::exists(getLayoutDir().filePath("DefaultWindowLayout"))) {
		mSaver.saveToFile(getLayoutDir().filePath("DefaultWindowLayout"));
	}
}

void EditorAttributeMgr::resetDefaultLayout()
{
	KDDockWidgets::LayoutSaver mSaver;
	mSaver.restoreFromFile(getLayoutDir().filePath("DefaultWindowLayout"));
}

QAbstractButton* WindowStyleFactory::createTitleBarButton(QWidget* parent, KDDockWidgets::TitleBarButtonType type) const
{
	QAbstractButton* button = nullptr;
	switch (type)
	{
	case KDDockWidgets::TitleBarButtonType::Close:
		button = new WinCloseButton;
		break;
	case KDDockWidgets::TitleBarButtonType::Float:
		button = new WinMaxButton;
		break;
	case KDDockWidgets::TitleBarButtonType::Minimize:
		button = new WinMinButton;
		break;
	case KDDockWidgets::TitleBarButtonType::Maximize:
		button = new WinMaxButton;
		break;
	case KDDockWidgets::TitleBarButtonType::Normal:
		break;
	case KDDockWidgets::TitleBarButtonType::AutoHide:
		break;
	case KDDockWidgets::TitleBarButtonType::UnautoHide:
		break;
	default:
		break;
	}
	if (button) {
		button->setFixedSize(21, 21);
	}
	return button ? button : new QPushButton();
}
using namespace KDDockWidgets;

KDDockWidgets::FloatingWindow* WindowStyleFactory::createFloatingWindow(KDDockWidgets::Frame* frame, KDDockWidgets::MainWindowBase* parent /*= nullptr*/, QRect suggestedGeometry /*= {}*/) const {
	KDDockWidgets::FloatingWindow* window = new FloatingWindowWidget(frame, suggestedGeometry, parent);
	QAeroWindowMaker::make((HWND)window->winId());
	return	window;
}
