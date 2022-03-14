#ifndef EngineMainWindow_h__
#define EngineMainWindow_h__

#include <kddockwidgets/MainWindow.h>
#include "Widgets/AssetPanel/AssetPanel.h"
#include "WindowLayoutMgr.h"

class EngineMainWindow : public KDDockWidgets::MainWindow {
	Q_OBJECT
public:
	EngineMainWindow();
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
	WindowAttributeMgr mWindowLayoutMgr;
};

#endif // EngineMainWindow_h__
