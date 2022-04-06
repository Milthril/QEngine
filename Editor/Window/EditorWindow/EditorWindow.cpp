#include "EditorWindow.h"
#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/Config.h>
#include <QMenuBar>
#include "QApplication"
#include "QStyleFactory"
#include "QEngine.h"
#include "Toolkit/QAeroWindowMaker.h"

EditorWindow::EditorWindow()
	: KDDockWidgets::FloatingWindowWidget({})
	, mMainWindow(KDDockWidgets::MainWindow("QEngine Editor"))
	, mAssetPanel(ASSET_DIR)
	, mFile("File")
	, mEdit("Edit")
	, mWindow("Window")
	, mQScenePanel(Engine->scene())
{
	createUI();
	connectUI();
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
		| KDDockWidgets::Config::Flag::Flag_LazyResize
		;

	KDDockWidgets::Config::self().setFlags(flags);
	KDDockWidgets::Config::self().setFrameworkWidgetFactory(new WindowStyleFactory);
}

void EditorWindow::createUI()
{
	mWindowLayoutMgr.initConfig();
	m_vlayout->addWidget(&mMainWindow);
	m_vlayout->setContentsMargins(0, 0, 0, 0);
	QAeroWindowMaker::make((HWND)this->winId());
	this->setObjectName("KDFloatingWindow");
	mMainWindow.setObjectName("KDMainWindow");
	mMainWindow.menuBar()->addMenu(&mFile);
	mMainWindow.menuBar()->addMenu(&mEdit);
	mMainWindow.menuBar()->addMenu(&mWindow);
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
	dockAssetPanel->setAffinities(mMainWindow.affinities());
	dockAssetPanel->setWidget(&mAssetPanel);
	mMainWindow.addDockWidget(dockAssetPanel, KDDockWidgets::Location::Location_OnBottom);

	auto dockScene = new KDDockWidgets::DockWidget("Scene", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockScene->setAffinities(mMainWindow.affinities());
	dockScene->setWidget(&mQScenePanel);
	mMainWindow.addDockWidget(dockScene, KDDockWidgets::Location::Location_OnTop);

	auto dockProperty = new KDDockWidgets::DockWidget("Property", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockProperty->setAffinities(mMainWindow.affinities());
	dockProperty->setWidget(&mPropertyPanel);
	mMainWindow.addDockWidget(dockProperty, KDDockWidgets::Location::Location_OnRight);

	auto dockViewport = new KDDockWidgets::DockWidget("Viewport", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockViewport->setAffinities(mMainWindow.affinities());
	mMainWindow.addDockWidget(dockViewport, KDDockWidgets::Location::Location_OnRight, dockScene);
	auto viewportContainter = QWidget::createWindowContainer(Engine->window().get());
	viewportContainter->setMinimumSize(400, 300);
	dockViewport->setWidget(viewportContainter);

	mWindowLayoutMgr.loadAllLayout();
	mWindowLayoutMgr.tryCreateDefaultLayout();
}

void EditorWindow::connectUI()
{
	connect(&mQScenePanel, &QScenePanel::objectChanged, &mPropertyPanel, &QPropertyPanel::setObject);
	connect(Engine, &QEngine::requestUpdatePropertyPanel, &mPropertyPanel, &QPropertyPanel::updatePanel);
}

void EditorWindow::showEvent(QShowEvent* event)
{
}