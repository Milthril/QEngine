#include "Window\EditorWindow\EditorWindow.h"
#include <QGuiApplication>
#include "QDateTime"
#include "QEngine.h"
#include "qrandom.h"
#include "Scene\Component\Camera\QCameraComponent.h"
#include "Scene\Component\Light\QLightComponent.h"
#include "Scene\Component\Particle\QParticleComponent.h"
#include "Scene\Component\SkyBox\QSkyBoxComponent.h"
#include "Scene\Component\StaticMesh\QCube.h"
#include "Scene\Component\StaticMesh\QSphere.h"
#include "Scene\Component\StaticMesh\QStaticModel.h"
#include "Scene\Component\StaticMesh\QText2D.h"
#include "Scene\Component\SkeletonMesh\QSkeletonMeshComponent.h"
#include "Scene\Component\AssimpToolkit\MMDVmdParser.h"
#include "Scene\Component\SkeletonMesh\QMMDModel.h"
#include "Window\MaterialEditor\QMaterialEditor.h"

const int CUBE_MAT_SIZE = 10;
const int CUBE_MAT_SPACING = 5;

class MyGame :public QEngine {
public:
	std::shared_ptr<QCameraComponent> mCamera;
	std::shared_ptr<QSkyBoxComponent> mSkyBox;
	std::shared_ptr<QCube> mCube[CUBE_MAT_SIZE][CUBE_MAT_SIZE];
	std::shared_ptr<QStaticModel> mStaticModel;
	std::shared_ptr<QSkeletonModelComponent> mSkeletonModel;
	std::shared_ptr<QParticleComponent> mGPUParticles;
	std::shared_ptr<QSphere> mSphere;
	std::shared_ptr<QText2D> mText;
	std::shared_ptr<QMaterial> mMaterial;

	QRandomGenerator rand;

	MyGame(int argc, char** argv)
		: QEngine(argc, argv) {
	}
	virtual void init() override {
		mCamera = std::make_shared<QCameraComponent>();
		mCamera->setupWindow(window().get());		//将相机与窗口绑定，使用WASD Shift 空格可进行移动，鼠标左键按住窗口可调整视角
		scene()->setCamera(mCamera);				//设置场景相机

		//mText = std::make_shared<QText2D>("GPU Particles");
		//mMaterial = std::make_shared<QMaterial>();
		//mMaterial->addParamVec3("BaseColor", QVector3D(0.1, 0.5, 0.9));					//设置材质参数
		//mMaterial->setShadingCode("FragColor = vec4(UBO.BaseColor,1);");				//设置材质的Shading代码
		//mText->setMaterial(mMaterial);

		//for (int i = 0; i < CUBE_MAT_SIZE; i++) {
		//	for (int j = 0; j < CUBE_MAT_SIZE; j++) {
		//		std::shared_ptr<QCube>& cube = mCube[i][j];
		//		cube.reset(new QCube);
		//		cube->setPosition(QVector3D((i - CUBE_MAT_SIZE / 2.0) * CUBE_MAT_SPACING, (j - CUBE_MAT_SIZE / 2.0) * CUBE_MAT_SPACING, -10));
		//		cube->setMaterial(mMaterial);
		//		scene()->addPrimitive(QString("cube(%1,%2)").arg(i).arg(j), cube);
		//	}
		//}

		//mStaticModel = std::make_shared<QStaticModel>();
		//mStaticModel->loadFromFile(R"(C:\Users\fuxinghao879\Desktop\QEngine\Example\Genji\Genji.FBX)");

		//mStaticModel->setRotation(QVector3D(-90, 0, 0));
		//scene()->addPrimitive("Genji", mStaticModel);

		//QMaterialEditor* editor = new QMaterialEditor(mMaterial);
		//editor->show();

		//mText->setPosition(QVector3D(0, -4, 0));
		//mText->setRotation(QVector3D(0, 180, 0));
		//scene()->addPrimitive("text", mText);

		mSkyBox = std::make_shared<QSkyBoxComponent>();
		mSkyBox->setSkyBoxImage(QImage(R"(C:\Users\fuxinghao879\Desktop\QEngine\Example\sky.jpeg)"));
		scene()->setSkyBox(mSkyBox);
		//scene()->addPrimitive("12", std::make_shared<QCube>());
		mGPUParticles = std::make_shared<QParticleComponent>();
		mGPUParticles->setStaticMesh(std::make_shared<QCube>());						//设置粒子的形状（实例）
		scene()->addPrimitive("GPU", mGPUParticles);
	}
protected:
	void update() override
	{
		float time = QTime::currentTime().msecsSinceStartOfDay();
		//auto anim = mSkeletonModel->getSkeleton()->getAnimations().first();
		//anim->show(fmod(time, anim->getDuration()));
		//mMaterial->setParam<QVector3D>("BaseColor", QVector3D(0.1, 0.5, 0.9) * (sin(time / 1000) * 5 + 5));		//设置材质颜色
		//mText->setScale(QVector3D(1, 1, 1) * (5 + 4 * sin(time / 1000)));										//RGB最大值超出1.0具有Bloom效果

		//QVector<QParticleComponent::Particle> particles(1000);													//每帧发射100个粒子，后续可定义各类发射器
		//for (auto& particle : particles) {																		//通过QParticleComponent::setUpdater可设置GPU粒子的运动代码，从而实现各种力的作用效果
		//	particle.position = QVector3D(rand.bounded(-10000, 10000) / 100.0, -200 + rand.bounded(-10000, 10000) / 100.0, rand.bounded(-10000, 10000) / 100.0).toVector4D();
		//	particle.velocity = QVector3D(rand.bounded(-10000, 10000) / 100.0, 0.1, 0);
		//}
		//mGPUParticles->createPartilces(particles);
	}
};
int main(int argc, char** argv) {
	MyGame engine(argc, argv);
	EditorWindow::preInitConfig();
	EditorWindow w;
	w.show();
	engine.execGame();
}