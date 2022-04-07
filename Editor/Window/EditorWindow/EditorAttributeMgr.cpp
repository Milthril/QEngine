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
#include "Toolkit\QAeroWindowMaker.h"
#include "QRegularExpression"
#include "Toolkit\QWidgetShadowMaker.h"
#include "QEngine.h"

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
	return Engine->assetDir().filePath("EditorStyle");
}

void EditorAttributeMgr::initDefaultAttribute() {
	if (mSaveDir.exists())
		return;
	mSaveDir.mkpath(getLayoutDir().path());
	mSaveDir.mkpath(getStyleSheetDir().path());;
	loadStyleSheet(getStyleSheetDir().filePath("Template.qss"));
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
	bool shadow = false;
	bool aero = false;
	QColor iconColor;
	if (file.open(QFile::ReadOnly)) {
		QString data = file.readAll();
		file.close();
		if (data.startsWith("QEngineEditor")) {
			QRegularExpression aeroRegExp("aero *: *(\\w+) *;");
			QRegularExpressionMatch aeroMatch = aeroRegExp.match(data.mid(0,100));
			if (aeroMatch.hasMatch()) {
				QStringList texts = aeroMatch.capturedTexts();
				if (texts.size() == 2 && texts.last() == "true") {
					aero = true;
				}
			}
			QRegularExpression shadowRegExp("shadow *: *(\\w+) *;");
			QRegularExpressionMatch shadowMatch = shadowRegExp.match(data.mid(0, 100));
			if (shadowMatch.hasMatch()) {
				QStringList texts = shadowMatch.capturedTexts();
				if (texts.size() == 2 && texts.last() == "true") {
					shadow = true;
				}
			}
			QRegularExpression iconColorRegExp("icon-color *: *rgba\\((.+)\\) *;");
			QRegularExpressionMatch iconColorMatch = iconColorRegExp.match(data.mid(0, 100));
			if (iconColorMatch.hasMatch()) {
				QStringList texts = iconColorMatch.capturedTexts();
				if (texts.size() == 2) {
					QStringList rgba = texts.last().split(',');
					if (rgba.size() == 4) {
						iconColor.setRed(rgba[0].toInt());
						iconColor.setGreen(rgba[1].toInt());
						iconColor.setBlue(rgba[2].toInt());
						iconColor.setAlpha(rgba[3].toInt());
					}
				}
			}
		}
		QWidgetShadowMaker::setEffectEnabled(shadow);
		QAeroWindowMaker::setEnabled(aero);
		QSvgIcon::setIconColor(iconColor);
		qApp->setStyleSheet(data);
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
	QAeroWindowMaker::make(window);
	return	window;
}
