#include "EngineMainWindow.h"
#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/Config.h>
#include <QMenuBar>

EngineMainWindow::EngineMainWindow()
	: KDDockWidgets::MainWindow("Axon Editor", KDDockWidgets::MainWindowOption::MainWindowOption_None)
	, mAssetPanel("F:/hero/game/assets")
	, mFile("File")
	, mEdit("Edit")
	, mWindow("Window")
{
	createUI();
}

void EngineMainWindow::preInitConfig()
{
	KDDockWidgets::Config::Flags flags = KDDockWidgets::Config::self().flags();
	flags = flags
		//| KDDockWidgets::Config::Flag_CloseOnlyCurrentTab
		| KDDockWidgets::Config::Flag_AllowReorderTabs
		| KDDockWidgets::Config::Flag_TitleBarHasMaximizeButton
		| KDDockWidgets::Config::Flag_TitleBarHasMinimizeButton
		| KDDockWidgets::Config::Flag_HideTitleBarWhenTabsVisible
		//| KDDockWidgets::Config::Flag_AlwaysShowTabs
		//| KDDockWidgets::Config::Flag_ShowButtonsOnTabBarIfTitleBarHidden
		| KDDockWidgets::Config::Flag_AlwaysTitleBarWhenFloating
		| KDDockWidgets::Config::Flag_TabsHaveCloseButton
		;

	KDDockWidgets::Config::self().setFlags(flags);
	KDDockWidgets::Config::self().setFrameworkWidgetFactory(new WindowStyleFactory);
}

void EngineMainWindow::createUI()
{
	menuBar()->addMenu(&mFile);
	menuBar()->addMenu(&mEdit);
	menuBar()->addMenu(&mWindow);

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

	KDDockWidgets::DockWidget::Options options = KDDockWidgets::DockWidget::Option_None;
	KDDockWidgets::DockWidget::LayoutSaverOptions layoutSaverOptions = KDDockWidgets::DockWidget::LayoutSaverOption::None;

	auto dockAssetPanel = new KDDockWidgets::DockWidget("Asset Panel", options, layoutSaverOptions);
	dockAssetPanel->setAffinities(affinities());
	dockAssetPanel->setWidget(&mAssetPanel);
	addDockWidget(dockAssetPanel, KDDockWidgets::Location::Location_OnTop);
}

void EngineMainWindow::connectUI()
{
}

void EngineMainWindow::showEvent(QShowEvent* event)
{
	mWindowLayoutMgr.initConfig();
}