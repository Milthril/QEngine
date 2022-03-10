#include <QGuiApplication>
#include "QDateTime"
#include "QEngine.h"
#include "qrandom.h"
#include "Render\Scene\Component\Camera\QCameraComponent.h"
#include "Render\Scene\Component\Light\QLightComponent.h"
#include "Render\Scene\Component\Particle\QParticleComponent.h"
#include "Render\Scene\Component\SkyBox\QSkyBoxComponent.h"
#include "Render\Scene\Component\StaticMesh\QCube.h"
#include "Render\Scene\Component\StaticMesh\QSphere.h"
#include "Render\Scene\Component\StaticMesh\QStaticModel.h"
#include "Render\Scene\Component\StaticMesh\QText2D.h"

class MyGame :public QEngine {
public:
	MyGame(int argc, char** argv)
		:QEngine(argc, argv)
	{
		cube = std::make_shared<QCube>();
		mCamera = std::make_shared<QCameraComponent>();
		mCamera->setupWindow(mWindow.get());
		mSkyBox = std::make_shared<QSkyBoxComponent>();
		mSkyBox->setSkyBoxImage(QImage(PROJECT_SOURCE_DIR"/sky.jpeg"));
		particles = std::make_shared<QParticleComponent>();
		scene()->addPrimitive(cube);
		scene()->setCamera(mCamera);
		scene()->setSkyBox(mSkyBox);

		int size = 10;
		int spacing = 5;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				auto element = std::make_shared<QCube>();
				element->setPosition(QVector3D( (i-size/2.0) * spacing, (j-size/2.0) * spacing, -10));
				scene()->addPrimitive(element);
			}
		}
		mStaticModel = std::make_shared<QStaticModel>();
		mStaticModel->loadFromFile(PROJECT_SOURCE_DIR"/Genji/Genji.FBX");
		mStaticModel->setRotation(QVector3D(-90, 0, 0));
		scene()->addPrimitive(mStaticModel);
	}
	std::shared_ptr<QStaticModel> mStaticModel;
	std::shared_ptr<QCameraComponent> mCamera;
	std::shared_ptr<QCube> cube;
	std::shared_ptr<QSkyBoxComponent> mSkyBox;
	std::shared_ptr<QParticleComponent> particles;
	std::shared_ptr<QSphere> sphere = std::make_shared<QSphere>();
	std::shared_ptr<QText2D> text = std::make_shared<QText2D>("default");

	std::shared_ptr<QMaterial> material;
	QRandomGenerator rand;
protected:
	void onGameLoop() override
	{
		float time = QTime::currentTime().msecsSinceStartOfDay();
		cube->getMaterial()->setParam<QVector4D>("BaseColor", QVector4D(1, 5, 9, 1));
		cube->setRotation(QVector3D(1, 1, 1) * time / 10.0);

	}
};

int main(int argc, char** argv) {
	MyGame game(argc, argv);
	game.execGame();
}