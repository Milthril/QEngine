#include "EditorWindow.h"
#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/Config.h>
#include <kddockwidgets/private/TitleBar_p.h>
#include <QMenuBar>
#include <QApplication>
#include <QScreen>
#include <QStyleFactory>
#include "Toolkit/QAeroWindowMaker.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"
#include "QEngineCoreApplication.h"
#include "Asset/GAssetMgr.h"

EditorWindow::EditorWindow()
	: KDDockWidgets::FloatingWindowWidget({})
	, mMainWindow(KDDockWidgets::MainWindow("QEngine Editor"))
	, mAssetPanel(TheAssetMgr->assetDir().path())
	, mFile("File")
	, mEdit("Edit")
	, mWindow("Window")
	, mScenePanel(TheEngine->world().get())
{
	createUI();
	connectUI();
}


EditorWindow::~EditorWindow() {
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
	this->titleBar()->setTitle("QEngine");
	mWindowLayoutMgr.initConfig();
	m_vlayout->addWidget(&mMainWindow);
	m_vlayout->setContentsMargins(0, 0, 0, 0);
	QAeroWindowMaker::make(this);
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

	auto dockConsolePanel = new KDDockWidgets::DockWidget("Console", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockConsolePanel->setAffinities(mMainWindow.affinities());
	dockConsolePanel->setWidget(&mConsolePanel);
	mMainWindow.addDockWidget(dockConsolePanel, KDDockWidgets::Location::Location_OnRight, nullptr,KDDockWidgets::InitialOption(QSize(180, 180)));

	auto dockScene = new KDDockWidgets::DockWidget("World", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockScene->setAffinities(mMainWindow.affinities());
	dockScene->setWidget(&mScenePanel);
	mMainWindow.addDockWidget(dockScene, KDDockWidgets::Location::Location_OnTop,nullptr, KDDockWidgets::InitialOption(QSize(800, 800)));

	auto dockEntity = new KDDockWidgets::DockWidget("Inspector", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockEntity->setAffinities(mMainWindow.affinities());
	dockEntity->setWidget(&mEntityPanel);
	mMainWindow.addDockWidget(dockEntity, KDDockWidgets::Location::Location_OnRight, dockScene, KDDockWidgets::InitialOption(QSize(120, 600)));

	auto dockViewport = new KDDockWidgets::DockWidget("Viewport", KDDockWidgets::DockWidget::Option_None, layoutSaverOptions);
	dockViewport->setAffinities(mMainWindow.affinities());
	mMainWindow.addDockWidget(dockViewport, KDDockWidgets::Location::Location_OnRight, dockScene, KDDockWidgets::InitialOption(QSize(900, 800)));
	dockViewport->setWidget(&mViewportPanel);

	mWindowLayoutMgr.loadAllLayout();
	mWindowLayoutMgr.tryCreateDefaultLayout();

	QRect rect(0, 0, 1200, 800);
	rect.moveCenter(qApp->primaryScreen()->geometry().center());
	setGeometry(rect);
}

void EditorWindow::connectUI(){
	connect(&mScenePanel, &QScenePanel::entityChanged, &mEntityPanel, &QEntityPanel::setEntity);
}

void EditorWindow::showEvent(QShowEvent* event)
{
}
