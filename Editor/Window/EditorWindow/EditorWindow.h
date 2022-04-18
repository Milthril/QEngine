#ifndef EditorWindow_h__
#define EditorWindow_h__

#include "EditorAttributeMgr.h"
#include "Widgets/AssetPanel/AssetPanel.h"
#include "Widgets/ScenePanel/QScenePanel.h"
#include "Widgets/EntityPanel/QEntityPanel.h"
#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/MainWindow.h>
#include "kddockwidgets/private/widgets/FloatingWindowWidget_p.h"

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
private:
	KDDockWidgets::MainWindow mMainWindow;
	QScenePanel mQScenePanel;
	QEntityPanel mEntityPanel;
	AssetPanel mAssetPanel;
	QMenu mFile;
	QMenu mEdit;
	QMenu mWindow;
	EditorAttributeMgr mWindowLayoutMgr;
	QWidget* mViewportContainter = nullptr;
};

#endif // EditorWindow_h__
