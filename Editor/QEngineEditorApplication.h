#ifndef QEngineEditorApplication_h__
#define QEngineEditorApplication_h__

#include "QEngineCoreApplication.h"
#include "Asset\GAssetMgr.h"
#include "Asset\SkyBox.h"
#include "ECS\Component\RenderableComponent\QSkyBoxComponent.h"
#include "ECS\Component\RenderableComponent\QStaticMeshComponent.h"
#include "Window\EditorWindow\EditorWindow.h"
#include "Asset\SkeletonModel\Skeleton.h"

void QEngineMessageHandler(QtMsgType, const QMessageLogContext&, const QString&);

#if defined(TheEngine)
#undef TheEngine
#endif
#define TheEngine (static_cast<QEngineEditorApplication *>(QEngineEditorApplication::instance()))

class QEngineEditorApplication :public QEngineCoreApplication {
public:

	QEngineEditorApplication(int argc, char** argv)
		: QEngineCoreApplication(argc, argv, true) {
		EditorWindow::preInitConfig();
		mEditorWindow = new EditorWindow;
		mEditorWindow->show();
		qInstallMessageHandler(QEngineMessageHandler);
	}
protected:
	virtual void customInit() override {
		QEntity* entity = world()->createEntity("Test Entity");
		auto staitcMeshAsset = TheAssetMgr->load<Asset::StaticMesh>("Genji Shim.QStaticMesh");
		QStaticMeshComponent* staitcMesh = entity->addComponent<QStaticMeshComponent>();
		staitcMesh->setStaticMesh(staitcMeshAsset);

		auto skyboxAsset = TheAssetMgr->load<Asset::SkyBox>("sky.QSkyBox");
		QSkyBoxComponent* skybox = entity->addComponent<QSkyBoxComponent>();
		skybox->setSkyBox(skyboxAsset);

		//auto it = TheAssetMgr->load<Asset::Skeleton>(assetDir().filePath("Genji Shim.QStaticMesh"));
	}
	void customUpdate() override {

	}
	virtual void customRelease() override {
		delete mEditorWindow;
	}
public:
	EditorWindow* getEditorWindow() const { return mEditorWindow; }
private:
	EditorWindow* mEditorWindow = nullptr;
};


#endif // QEngineEditorApplication_h__
