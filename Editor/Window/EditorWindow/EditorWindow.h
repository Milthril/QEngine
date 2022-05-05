#ifndef EditorWindow_h__
#define EditorWindow_h__

#include "EditorAttributeMgr.h"
#include "Widgets/AssetPanel/AssetPanel.h"
#include "Widgets/ScenePanel/QScenePanel.h"
#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/MainWindow.h>
#include "kddockwidgets/private/widgets/FloatingWindowWidget_p.h"
#include "Widgets/ViewportPanel/ViewportPanel.h"
#include "Widgets/ConsolePanel/QConsolePanel.h"
#include "Widgets/Inspector/QInspector.h"

class EditorWindow : public KDDockWidgets::FloatingWindowWidget{
	Q_OBJECT
public:
	EditorWindow();
	~EditorWindow();
	static void preInitConfig();
protected:
	void createUI();
	void connectUI();
	void showEvent(QShowEvent* event) override;
public:
	KDDockWidgets::MainWindow mMainWindow;
	QScenePanel mScenePanel;
	QInspector mInspector;
	QConsolePanel mConsolePanel;
	AssetPanel mAssetPanel;
	ViewportPanel mViewportPanel;
	QMenu mFile;
	QMenu mEdit;
	QMenu mWindow;
	EditorAttributeMgr mWindowLayoutMgr;
};

#endif // EditorWindow_h__
