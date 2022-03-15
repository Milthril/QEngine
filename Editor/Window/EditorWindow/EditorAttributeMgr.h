#ifndef EditorAttributeMgr_h__
#define EditorAttributeMgr_h__

#include <QMenu>
#include <QDir>
#include <QJsonObject>
#include "kddockwidgets/KDDockWidgets.h"
#include "kddockwidgets/LayoutSaver.h"
#include "kddockwidgets/FrameworkWidgetFactory.h"

class EditorAttributeMgr {
	friend class EditorWindow;
public:
	enum class WindowStyle;
	EditorAttributeMgr();
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



#endif // EditorAttributeMgr_h__
