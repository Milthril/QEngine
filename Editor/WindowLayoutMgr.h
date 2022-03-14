#ifndef WindowLayoutMgr_h__
#define WindowLayoutMgr_h__

#include <QMenu>
#include <QDir>
#include "kddockwidgets/KDDockWidgets.h"
#include "kddockwidgets/LayoutSaver.h"
#include "kddockwidgets/FrameworkWidgetFactory.h"
#include "QJsonObject"

class WindowAttributeMgr {
	friend class EngineMainWindow;
public:
	enum class WindowStyle;
	WindowAttributeMgr();
	void initConfig();
	void saveCurrentLayout();
	void resetDefaultLayout();
	QDir getLayoutDir();
	QDir getStyleSheetDir();
protected:
	void initDefaultAttribute();
	void writeFile(QString filePath, QByteArray data);
	void loadAllLayout();
	void loadAllStyleSheet();
	void loadStyleSheet(QString filePath);
	void saveCurrentCtx();
	void loadCurrentCtx();
private:
	QMenu* mLayoutMenu = new QMenu("Restore Window Layout");
	QMenu* mWindowStyleMenu = new QMenu("Window Style");
	QDir mSaveDir = QDir::current().filePath("Config/WindowAttribute");
	QMap<QString, QAction*> mLayoutActionMap;
	QJsonObject mCurrentCtx;
	KDDockWidgets::LayoutSaver mSaver;
};

class WindowStyleFactory :public KDDockWidgets::DefaultWidgetFactory {
public:
	QAbstractButton* createTitleBarButton(QWidget* parent, KDDockWidgets::TitleBarButtonType) const override;
};

#endif // WindowLayoutMgr_h__
