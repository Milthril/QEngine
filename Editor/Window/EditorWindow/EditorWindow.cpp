#include "EditorWindow.h"
#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/Config.h>
#include <QMenuBar>
#include "QApplication"
#include "QStyleFactory"
#include "QEngine.h"

EditorWindow::EditorWindow()
	: KDDockWidgets::MainWindow("QEngine Editor")
	, mAssetPanel("F:/hero/game/assets")
	, mFile("File")
	, mEdit("Edit")
	, mWindow("Window")
{
	createUI();
}

void EditorWindow::preInitConfig()
{
	qApp->setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
	KDDockWidgets::Config::Flags flags = KDDockWidgets::Config::self().flags();
	flags = flags
		//| KDDockWidgets::Config::Flag_CloseOnlyCurrentTab
		| KDDockWidgets::Config::Flag_AllowReorderTabs
		| KDDockWidgets::Config::Flag_TitleBarHasMaximizeButton
		| KDDockWidgets::Config::Flag_TitleBarHasMinimizeButton
		| KDDockWidgets::Config::Flag_AlwaysShowTabs
		| KDDockWidgets::Config::Flag_ShowButtonsOnTabBarIfTitleBarHidden
		//| KDDockWidgets::Config::Flag_AlwaysTitleBarWhenFloating
		//| KDDockWidgets::Config::Flag_TabsHaveCloseButton
		;

	KDDockWidgets::Config::self().setFlags(flags);
	KDDockWidgets::Config::self().setFrameworkWidgetFactory(new WindowStyleFactory);
}

void EditorWindow::createUI()
{
	menuBar()->addMenu(&mFile);
	menuBar()->addMenu(&mEdit);
	menuBar()->addMenu(&mWindow);
	this->
		mWindow.addAction("New Window", [this]() {
		static int counter;
		auto dock = new KDDockWidgets::DockWidget(QStringLiteral("new dock %1").arg(counter++));
		dock->show();
	});
	mWindow.addMenu(mWindowLayoutMgr.mWindowStyleMenu);
	mWindow.addSection("Layout");
	mWindow.addAction("Save Window Layout", [this]() {
		mWindowLayoutMgr.saveCurrentLayout();
	});
	mWindow.addMenu(mWindowLayoutMgr.mLayoutMenu);
	mWindow.addAction("Reset Window Layout", [this]() {
		mWindowLayoutMgr.resetDefaultLayout();
	});

	KDDockWidgets::DockWidget::LayoutSaverOptions layoutSaverOptions = KDDockWidgets::DockWidget::LayoutSaverOption::None;

	auto dockAssetPanel = new KDDockWidgets::DockWidget("Asset", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockAssetPanel->setAffinities(affinities());
	dockAssetPanel->setWidget(&mAssetPanel);
	addDockWidget(dockAssetPanel, KDDockWidgets::Location::Location_OnBottom);

	auto dockViewport = new KDDockWidgets::DockWidget("Viewport", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockViewport->setAffinities(affinities());
	auto viewportContainter = QWidget::createWindowContainer(Engine->window().get());
	viewportContainter->setMinimumSize(400, 300);
	dockViewport->setWidget(viewportContainter);
	addDockWidget(dockViewport, KDDockWidgets::Location::Location_OnTop);
}

void EditorWindow::connectUI()
{
}

void EditorWindow::showEvent(QShowEvent* event)
{
	mWindowLayoutMgr.initConfig();
}