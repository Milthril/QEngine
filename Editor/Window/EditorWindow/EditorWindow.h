#ifndef EditorWindow_h__
#define EditorWindow_h__

#include <kddockwidgets/MainWindow.h>
#include "Widgets/AssetPanel/AssetPanel.h"
#include "EditorAttributeMgr.h"
#include <kddockwidgets/DockWidget.h>

class EditorWindow : public KDDockWidgets::MainWindow {
	Q_OBJECT
public:
	EditorWindow();
	static void preInitConfig();
protected:
	void createUI();
	void connectUI();
	void showEvent(QShowEvent* event) override;
private:
	AssetPanel mAssetPanel;
	QMenu mFile;
	QMenu mEdit;
	QMenu mWindow;
	EditorAttributeMgr mWindowLayoutMgr;
};

#endif // EditorWindow_h__
