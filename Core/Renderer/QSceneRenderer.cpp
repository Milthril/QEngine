#include "QSceneRenderer.h"
#include "Scene/Component/QPrimitiveComponent.h"
#include "Scene/Component/StaticMesh/QStaticMeshComponent.h"
#include "Scene/Component/SkeletonMesh/QSkeletonMeshComponent.h"
#include "Scene/Component/Light/QLightComponent.h"
#include "Scene/Component/SkyBox/QSkyBoxComponent.h"
#include "Scene/Component/Particle/QParticleComponent.h"
#include "Scene/Component/Camera/QCameraComponent.h"
#include "private/qshaderbaker_p.h"
#include "RHI/QRhiUniformMgr.h"
#include "QEngine.h"

QSceneRenderer::QSceneRenderer(int sampleCount, QRhiSPtr<QRhiRenderPassDescriptor> renderPassDescriptor)
	: mSampleCount(sampleCount)
	, mRootRenderPassDescriptor(renderPassDescriptor)
	, mBloomPainter(new QBloomPainter())
{
}

void QSceneRenderer::setScene(std::shared_ptr<QScene> scene)
{
	if (mScene) {
		mScene->disconnect();
	}
	mScene = scene;
	connect(mScene.get(), &QScene::lightChanged, this, &QSceneRenderer::onLightChanged);
	connect(mScene.get(), &QScene::primitiveInserted, this, &QSceneRenderer::onPrimitiveInserted);
	connect(mScene.get(), &QScene::primitiveRemoved, this, &QSceneRenderer::onPrimitiveRemoved);
}

void QSceneRenderer::renderInternal(QRhiCommandBuffer* buffer, QRhiRenderTarget* renderTarget)
{
	QRhiResourceUpdateBatch* batch = RHI->nextResourceUpdateBatch();
	tryResetUniformProxy();
	tryResetPrimitiveProxy();
	tryResetSkyBox(batch);
	for (auto& it : mProxyUploadList)
		it->uploadResource(batch);

	mProxyUploadList.clear();
	QRhiUniformMgr::instance()->update(batch);
	buffer->resourceUpdate(batch);

	render(buffer, renderTarget);
}

QMatrix4x4 QSceneRenderer::getViewMatrix()
{
	if (mScene->mCamera) {
		return mScene->mCamera->getMatrixView();
	}
	QMatrix4x4 matrix;
	return matrix;
}

QMatrix4x4 QSceneRenderer::getClipMatrix() const
{
	if (mScene->mCamera) {
		return mScene->mCamera->getMatrixClip();
	}
	return QMatrix4x4();
}

QMatrix4x4 QSceneRenderer::getVP()
{
	if (mScene->mCamera) {
		return mScene->mCamera->getMatrixVP();
	}
	return QMatrix4x4();
}

QShader QSceneRenderer::createShaderFromCode(QShader::Stage stage, const char* code)
{
	QShaderBaker baker;
	baker.setGeneratedShaderVariants({ QShader::StandardShader });
	baker.setGeneratedShaders({
		QShaderBaker::GeneratedShader{QShader::Source::SpirvShader,QShaderVersion(100)},
		QShaderBaker::GeneratedShader{QShader::Source::GlslShader,QShaderVersion(450)},
		QShaderBaker::GeneratedShader{QShader::Source::MslShader,QShaderVersion(12)},
		QShaderBaker::GeneratedShader{QShader::Source::HlslShader,QShaderVersion(50)},
	});

	baker.setSourceString(code, stage);
	QShader shader = baker.bake();
	if (!shader.isValid())
		qFatal(baker.errorMessage().toLocal8Bit());
	return shader;
}

void QSceneRenderer::onPrimitiveInserted(uint32_t index, std::shared_ptr<QPrimitiveComponent> primitive)
{
	for (auto& child : primitive->mChildren) {
		onPrimitiveInserted(-1, std::dynamic_pointer_cast<QPrimitiveComponent>(child));
	}
	std::shared_ptr<QRhiProxy> proxy = createPrimitiveProxy(primitive);
	if (!proxy)
		return;
	mPrimitiveProxyMap[primitive->componentId()] = proxy;
	mProxyUploadList << proxy;
}

void QSceneRenderer::onPrimitiveRemoved(std::shared_ptr<QPrimitiveComponent> primitive)
{
	mPrimitiveProxyMap.remove(primitive->componentId());
}

void QSceneRenderer::onLightChanged()
{
}

void QSceneRenderer::onSkyBoxChanged()
{
	mSkyBoxProxy.reset();
}

void QSceneRenderer::tryResetUniformProxy()
{
	for (auto& uniform : QRhiUniformMgr::instance()->mUniformMap) {
		if (uniform->bNeedRecreate.receive()) {
			const auto& proxy = uniform->getProxy();
			proxy->recreateResource();
		}
	}
}

void QSceneRenderer::tryResetSkyBox(QRhiResourceUpdateBatch* batch)
{
	if (!mSkyBoxProxy && mScene->getSkyBox()) {
		mSkyBoxProxy = createSkyBoxProxy(mScene->getSkyBox());
		mSkyBoxProxy->mRenderer = this;
		mSkyBoxProxy->mComponent = mScene->getSkyBox();
		mSkyBoxProxy->recreateResource();
		mSkyBoxProxy->recreatePipeline();
		mSkyBoxProxy->uploadResource(batch);
	}
}

void QSceneRenderer::tryResetPrimitiveProxy()
{
	for (auto& id : mPrimitiveProxyMap.keys()) {
		auto& proxy = mPrimitiveProxyMap[id];
		if (proxy->mComponent->bNeedResetProxy.receive()) {
			proxy->recreateResource();
			proxy->recreatePipeline();
		}
	}
}

std::shared_ptr<QRhiProxy> QSceneRenderer::createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component)
{
	if (!component)
		return nullptr;
	std::shared_ptr<QRhiProxy> proxy;
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
	proxy->mRenderer = this;
	proxy->mComponent = component;
	component->bNeedResetProxy.active();
	return std::dynamic_pointer_cast<QRhiProxy>(proxy);
}

void QSceneRenderer::resetPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component)
{
	std::shared_ptr<QRhiProxy> newProxy = createPrimitiveProxy(component);
	newProxy->mComponent = component;
	newProxy->mRenderer = this;
	newProxy->recreateResource();
	newProxy->recreatePipeline();
	mPrimitiveProxyMap[component->componentId()] = newProxy;
	mProxyUploadList << newProxy;
}