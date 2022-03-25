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
#include "Scene\Component\Particle\PositionGenerator\QCubeGenerator.h"
#include "Scene\Component\Particle\QParticleEmitter.h"
#include "QDir"

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
	std::shared_ptr<QText2D> mText;
	std::shared_ptr<QMaterial> mMaterial;
	QRandomGenerator rand;
	MyGame(int argc, char** argv)
		: QEngine(argc, argv) {
	}
	void init() override {
		QDir assetDir(ASSET_DIR);
		if (!assetDir.exists()) {
			assetDir.setPath("../Asset");
		}

		mCamera = std::make_shared<QCameraComponent>();
		mCamera->setupWindow(window().get());		//将相机与窗口绑定，使用WASD Shift 空格可进行移动，鼠标左键按住窗口可调整视角
		scene()->setCamera(mCamera);				//设置场景相机

		mSkyBox = std::make_shared<QSkyBoxComponent>();
		mSkyBox->setSkyBoxImage(QImage(assetDir.filePath("sky.jpeg")));
		scene()->setSkyBox(mSkyBox);

		mMaterial = std::make_shared<QMaterial>();																	//新建材质
		mMaterial->addDataVec3("BaseColor", QVector3D(0.1, 0.5, 0.9));												//添加材质参数
		mMaterial->setShadingCode("FragColor = vec4(UBO.BaseColor,1);");											//设置材质的Shading代码

		mGPUParticles = std::make_shared<QParticleComponent>();
		mGPUParticles->getParticleSystem()->getUpdater()->addDataVec3("force", QVector3D(0, 0.0098, 0));			//为粒子添加一个作用力，设置粒子的运动代码
		mGPUParticles->getParticleSystem()->getUpdater()->setUpdateCode(R"(
			outParticle.position = inParticle.position + inParticle.velocity;
			outParticle.velocity = inParticle.velocity + UBO.force;
			outParticle.scaling  = inParticle.scaling;
			outParticle.rotation = inParticle.rotation;
		)");

		std::shared_ptr<QCubeGenerator> positionGenerator = std::make_shared<QCubeGenerator>();						//设置粒子的位置生成器，这部分用到了SubClass结构，可扩展
		positionGenerator->setWidth(1000);
		positionGenerator->setHeight(1000);
		positionGenerator->setDepth(1000);
		mGPUParticles->getParticleSystem()->getEmitter()->setPositionGenerator(positionGenerator);					//设置粒子存活时间
		mGPUParticles->getParticleSystem()->getEmitter()->setLifetime(2);
		mGPUParticles->getParticleSystem()->getEmitter()->setNumOfTick(1000.0);										//设置每帧发射数量，发射方式可扩展
		mGPUParticles->getParticleSystem()->getEmitter()->setScaling(QVector3D(0.05, 0.05, 0.05));
		mGPUParticles->setStaticMesh(std::make_shared<QCube>());													//设置粒子的形状（实例），可扩展
		mGPUParticles->getStaticMesh()->setMaterial(mMaterial);														//设置粒子材质
		scene()->addPrimitive("GPU particles", mGPUParticles);

		for (int i = 0; i < CUBE_MAT_SIZE; i++) {
			for (int j = 0; j < CUBE_MAT_SIZE; j++) {
				std::shared_ptr<QCube>& cube = mCube[i][j];
				cube.reset(new QCube);
				cube->setPosition(QVector3D((i - CUBE_MAT_SIZE / 2.0) * CUBE_MAT_SPACING, (j - CUBE_MAT_SIZE / 2.0) * CUBE_MAT_SPACING, -10));
				scene()->addPrimitive(QString("Cube(%1%,%2)").arg(i).arg(j), cube);
			}
		}

		mText = std::make_shared<QText2D>("GPU Particles");															//设置文字材质
		mText->setMaterial(mMaterial);
		mText->setPosition(QVector3D(0, -4, 0));
		mText->setRotation(QVector3D(0, 180, 0));
		scene()->addPrimitive("Text", mText);

		mStaticModel = std::make_shared<QStaticModel>();
		mStaticModel->loadFromFile(assetDir.filePath("Model/FBX/Genji/Genji.FBX"));									//通过Assimp加载模型，材质也是根据Assmip解析创建默认的QMaterial
		mStaticModel->setRotation(QVector3D(-90, 0, 0));
		scene()->addPrimitive("Model", mStaticModel);
	}
	void update() override
	{
		float time = QTime::currentTime().msecsSinceStartOfDay();

		mMaterial->setData<QVector3D>("BaseColor", QVector3D(0.1, 0.5, 0.9) * (sin(time / 1000) * 10 + 10));			//设置材质呼吸色，RGB最大值超出1.0具有Bloom效果

		//设置粒子的作用力参数
		mGPUParticles->getParticleSystem()->getUpdater()->setData<QVector3D>("force", QVector3D(cos(time / 1000), sin(time / 1000), 0.9) * 0.01);
	}
};

int main(int argc, char** argv) {
	MyGame game(argc, argv);
	game.execGame();
}