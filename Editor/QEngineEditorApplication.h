#ifndef QEngineEditorApplication_h__
#define QEngineEditorApplication_h__

#include "QEngineCoreApplication.h"
#include "Asset\GAssetMgr.h"
#include "Asset\SkyBox.h"
#include "ECS\Component\RenderableComponent\QSkyBoxComponent.h"
#include "ECS\Component\RenderableComponent\QStaticMeshComponent.h"
#include "Window\EditorWindow\EditorWindow.h"

class QEngineEditorApplication :public QEngineCoreApplication {
public:

	QEngineEditorApplication(int argc, char** argv)
		: QEngineCoreApplication(argc, argv, true) {
		EditorWindow::preInitConfig();
		mEditorWindow = new EditorWindow;
		mEditorWindow->show();
	}
protected:
	virtual void customInit() override {
		QEntity* entity = world()->createEntity("Test Entity");

		auto staitcMeshAsset = TheAssetMgr->load<Asset::StaticMesh>(assetDir().filePath("Genji Shim.QStaticMesh"));
		QStaticMeshComponent* staitcMesh = entity->addComponent<QStaticMeshComponent>();
		staitcMesh->setStaticMesh(staitcMeshAsset);

		auto skyboxAsset = TheAssetMgr->load<Asset::SkyBox>(assetDir().filePath("sky.QSkyBox"));

		QSkyBoxComponent* skybox = entity->addComponent<QSkyBoxComponent>();
		skybox->setSkyBox(skyboxAsset);
	}
	void customUpdate() override {
	}
private:
	EditorWindow* mEditorWindow = nullptr;
};


#endif // QEngineEditorApplication_h__
