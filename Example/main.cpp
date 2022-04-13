#include <QGuiApplication>
#include "QDateTime"
#include "QEngine.h"
#include "qrandom.h"
#include "QDir"
#include "TimeDomainProvider.h"
#include "Asset\Importer\QAssetImporter.h"
#include "ECS\Component\QCameraComponent.h"
#include "ECS\Component\RenderableComponent\QStaticMeshComponent.h"
#include "ECS\Component\RenderableComponent\QSkyBoxComponent.h"

const int CUBE_MAT_SIZE = 10;
const int CUBE_MAT_SPACING = 5;

class MyGame :public QEngine {
public:
	MyGame(int argc, char** argv)
		: QEngine(argc, argv) {
	}
	void customInit() override {

		//QAssetImpoerter::instance()->import(R"(C:\Users\fuxinghao879\Desktop\QEngine\Asset\Model\FBX\Genji\Genji.fbx)", assetDir());
		//QAssetImpoerter::instance()->import(R"(C:\Users\fuxinghao879\Desktop\QEngine\Asset\sky.jpeg)", assetDir());

		QEntity* entity = world()->createEntity("Entity");
		auto staitcMeshAsset = QAssetImpoerter::instance()->load<Asset::StaticMesh>(assetDir().filePath("Genji Shim.QAsset"));

		QStaticMeshComponent* staitcMesh = entity->addComponent<QStaticMeshComponent>();
		staitcMesh->setStaticMesh(staitcMeshAsset);

		auto skyboxAsset  = QAssetImpoerter::instance()->load<Asset::SkyBox>(assetDir().filePath("sky.QAsset"));
		
		QSkyBoxComponent* skybox = entity->addComponent<QSkyBoxComponent>();
		skybox->setSkyBox(skyboxAsset);

	}
	void customUpdate() override
	{
	}
};

int main(int argc, char** argv) {
	MyGame game(argc, argv);
	game.execGame();
}