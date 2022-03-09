#include <QGuiApplication>
#include "QEngine.h"
#include "QDateTime"
#include "Render\Scene\Component\CustomShape\QCube.h"
#include "Render\Scene\Component\QLightComponent.h"
#include "Render\Scene\Component\CustomShape\QSphere.h"
#include "Render\Scene\Component\CustomShape\QText2D.h"
#include "Render\Scene\Component\QParticleComponent.h"
#include "Render\Scene\Component\QSkyBoxComponent.h"
#include "Render\Scene\Component\Camera\QCameraComponent.h"
#include "qrandom.h"

class MyGame :public QEngine {
public:

	MyGame(int argc, char** argv)
		:QEngine(argc, argv)
	{
		cube = std::make_shared<QCube>();
		mCamera = std::make_shared<QCameraComponent>();
		mCamera->setupWindow(mWindow.get());
		mSkyBox = std::make_shared<QSkyBoxComponent>();
		mSkyBox->setSkyBoxImage(QImage(R"(C:\Users\fuxinghao879\Desktop\QEngine\Core\Demo\sky.jpeg)"));
		particles = std::make_shared<QParticleComponent>();
		//scene()->addPrimitive(cube);
		scene()->setCamera(mCamera);
		scene()->setSkyBox(mSkyBox);
	}
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

		cube->setRotation(QVector3D(1, 1, 1) * time / 10.0);
	}
};

int main(int argc, char** argv) {
	MyGame game(argc, argv);
	game.execGame();
}