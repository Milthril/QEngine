#include "ISceneRenderer.h"

#include "CommonPass/DebugDrawPass.h"
#include "ISceneComponentRenderProxy.h"
#include "private/qshaderbaker_p.h"
#include "QEngine.h"
#include "RHI/QRhiUniformMgr.h"
#include "Scene/Component/Camera/QCameraComponent.h"
#include "Scene/Component/Light/QLightComponent.h"
#include "Scene/Component/Particle/QParticleComponent.h"
#include "Scene/Component/QPrimitiveComponent.h"
#include "Scene/Component/SkeletonMesh/QSkeletonMeshComponent.h"
#include "Scene/Component/SkyBox/QSkyBoxComponent.h"
#include "Scene/Component/StaticMesh/QStaticMeshComponent.h"
ISceneRenderer::ISceneRenderer()
{
}

void ISceneRenderer::setScene(std::shared_ptr<QScene> scene)
{
	if (mScene) {
		mScene->disconnect();
	}
	mScene = scene;
}

QShader ISceneRenderer::createShaderFromCode(QShader::Stage stage, const char* code)
{
	QShaderBaker baker;
	baker.setGeneratedShaderVariants({ QShader::StandardShader });
	baker.setGeneratedShaders({
		QShaderBaker::GeneratedShader{QShader::Source::SpirvShader,QShaderVersion(100)},
		QShaderBaker::GeneratedShader{QShader::Source::GlslShader,QShaderVersion(430)},
		QShaderBaker::GeneratedShader{QShader::Source::MslShader,QShaderVersion(12)},
		QShaderBaker::GeneratedShader{QShader::Source::HlslShader,QShaderVersion(50)},
							  });

	baker.setSourceString(code, stage);
	QShader shader = baker.bake();
	if (!shader.isValid()) {
		qWarning(code);
		qWarning(baker.errorMessage().toLocal8Bit());
	}

	return shader;
}

std::shared_ptr<ISceneComponentRenderProxy> ISceneRenderer::createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component)
{
	if (!component)
		return nullptr;
	std::shared_ptr<ISceneComponentRenderProxy> proxy;
	switch (component->type())
	{
	case QSceneComponent::None:
		return nullptr;
		break;
	case QSceneComponent::StaticMesh:
		proxy = createStaticMeshProxy(std::dynamic_pointer_cast<QStaticMeshComponent>(component));
		break;
	case QSceneComponent::SkeletonMesh:
		proxy = createSkeletonMeshProxy(std::dynamic_pointer_cast<QSkeletonModelComponent>(component));
		break;
	case QSceneComponent::Particle:
		proxy = createParticleProxy(std::dynamic_pointer_cast<QParticleComponent>(component));
		break;
	default:
		return nullptr;
		break;
	}
	proxy->mComponent = component;
	component->bNeedRecreateResource.active();
	component->bNeedRecreatePipeline.active();
	return std::dynamic_pointer_cast<ISceneComponentRenderProxy>(proxy);
}