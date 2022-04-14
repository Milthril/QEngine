#include "Window\EditorWindow\EditorWindow.h"
#include <QGuiApplication>
#include "QDateTime"
#include "QEngine.h"
#include "Asset\StaticMesh.h"

#include "ECS\Component\RenderableComponent\QStaticMeshComponent.h"
#include "ECS\Component\RenderableComponent\QSkyBoxComponent.h"

class MyGame :public QEngine {
public:

	MyGame(int argc, char** argv)
		: QEngine(argc, argv, true) {
	}
	virtual void customInit() override {
		QEntity* entity = world()->createEntity("Test Entity");

		auto staitcMeshAsset = IAsset::LoadAsset<Asset::StaticMesh>(assetDir().filePath("Genji Shim.QStaticMesh"));
		QStaticMeshComponent* staitcMesh = entity->addComponent<QStaticMeshComponent>();
		staitcMesh->setStaticMesh(staitcMeshAsset);

		auto skyboxAsset = IAsset::LoadAsset<Asset::SkyBox>(assetDir().filePath("sky.QSkyBox"));

		QSkyBoxComponent* skybox = entity->addComponent<QSkyBoxComponent>();
		skybox->setSkyBox(skyboxAsset);
	}
protected:
	void customUpdate() override
	{
	}
};
int main(int argc, char** argv) {
	MyGame engine(argc, argv);
	EditorWindow::preInitConfig();
	EditorWindow w;
	w.show();
	engine.execGame();
}