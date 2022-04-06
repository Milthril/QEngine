#ifndef EditorWindow_h__
#define EditorWindow_h__

#include "EditorAttributeMgr.h"
#include "Widgets/AssetPanel/AssetPanel.h"
#include "Widgets/PropertyPanel/QPropertyPanel.h"
#include "Widgets/ScenePanel/QScenePanel.h"
#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/MainWindow.h>
#include "../WindowToolkit/FramlessWindow.h"
#include "kddockwidgets/private/widgets/FloatingWindowWidget_p.h"

class EditorWindow : public KDDockWidgets::FloatingWindowWidget{
	Q_OBJECT
public:
	EditorWindow();
	static void preInitConfig();
protected:
	void createUI();
	void connectUI();
	void showEvent(QShowEvent* event) override;
private:
	KDDockWidgets::MainWindow mMainWindow;
	QScenePanel mQScenePanel;
	QPropertyPanel mPropertyPanel;
	AssetPanel mAssetPanel;
	QMenu mFile;
	QMenu mEdit;
	QMenu mWindow;
	EditorAttributeMgr mWindowLayoutMgr;
};

#endif // EditorWindow_h__
