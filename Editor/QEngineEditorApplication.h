#ifndef QEngineEditorApplication_h__
#define QEngineEditorApplication_h__

#include "QEngineCoreApplication.h"
#include "Asset\GAssetMgr.h"
#include "Asset\SkyBox.h"
#include "Window\EditorWindow\EditorWindow.h"
#include "Asset\SkeletonModel\Skeleton.h"
#include "Window\ParticlesEditor\QParticlesEditor.h"
#include "Window\MaterialEditor\QMaterialEditor.h"

#include "ECS\Component\RenderableComponent\QSkyBoxComponent.h"
#include "ECS\Component\RenderableComponent\QStaticMeshComponent.h"
#include "ECS\Component\RenderableComponent\QSkeletonModelComponent.h"
#include "ECS\Component\RenderableComponent\Shape\QSpectrumComponent.h"
#include "ECS\Component\RenderableComponent\QParticleSystemComponent.h"

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
		//qInstallMessageHandler(QEngineMessageHandler);
	}
protected:
	virtual void customInit() override {
		QEntity* skybox = world()->createEntity("Skybox");
		QSkyBoxComponent* skyboxComp = skybox->addComponent<QSkyBoxComponent>();
		skyboxComp->setSkyBox(TheAssetMgr->load<Asset::SkyBox>("SkyBox.QSkyBox"));

		for (int i = 0; i < 13; i++) {
			QEntity* staticMesh = world()->createEntity(QString("StaticMesh%1").arg(i));
			QStaticMeshComponent* staitcMeshComp = staticMesh->addComponent<QStaticMeshComponent>();
			staitcMeshComp->setStaticMesh(TheAssetMgr->load<Asset::StaticMesh>(QString("Mesh (%1).QStaticMesh").arg(i)));
		}

		QEntity * skeletonModel = world()->createEntity("SkeletonModel");
		QSkeletonModelComponent* skeletonModelComp = skeletonModel->addComponent<QSkeletonModelComponent>();
		skeletonModelComp->setSkeletonModel(TheAssetMgr->load<Asset::SkeletonModel>("Catwalk Walk Turn 180 Tight R.QSkeletonModel"));
		skeletonModelComp->setAnimation(TheAssetMgr->load<Asset::SkeletonAnimation>("SkeletonAnimation.QSkeletonAnimation"));

		QEntity* spectrum = world()->createEntity("Audio Spectrum");
		QSpectrumComponent* spectrumComp = spectrum->addComponent<QSpectrumComponent>();

		QEntity* particleSystem = world()->createEntity("GPU Particles");
		QParticleSystemComponent* particleSystemComp = particleSystem->addComponent<QParticleSystemComponent>();
		particleSystemComp->setStaticMesh(TheAssetMgr->load<Asset::StaticMesh>("Cube.QStaticMesh"));
	
	}
	void customUpdate() override {

	}
	virtual void customRelease() override {
		//qInstallMessageHandler(nullptr);
		delete mEditorWindow;
		QParticlesEditor::instance()->shutdown();
		QMaterialEditor::instance()->shutdown();
	}
public:
	EditorWindow* getEditorWindow() const { return mEditorWindow; }
private:
	EditorWindow* mEditorWindow = nullptr;
};


#endif // QEngineEditorApplication_h__
