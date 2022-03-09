#include "QSceneRenderer.h"
#include "Render/Scene/Component/QPrimitiveComponent.h"
#include "Render/Scene/Component/QStaticMeshComponent .h"
#include "Render/Scene/Component/QShapeComponent.h"
#include "Render/Scene/Component/QSkeletonMeshComponent.h"
#include "Render/Scene/Component/QLightComponent.h"
#include "Render/Scene/Component/QSkyBoxComponent.h"
#include "Render/Scene/Component/QParticleComponent.h"
#include "Render/Scene/Component/Camera/QCameraComponent.h"
#include "private/qshaderbaker_p.h"

QSceneRenderer::QSceneRenderer(std::shared_ptr<QRhi> rhi, int sampleCount, QRhiSPtr<QRhiRenderPassDescriptor> renderPassDescriptor)
	: mRhi(rhi)
	, mSampleCount(sampleCount)
	, mRootRenderPassDescriptor(renderPassDescriptor)
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

void QSceneRenderer::setRenderTargetSize(QSize size)
{
	mRTSize = size;
}

void QSceneRenderer::renderInternal(QRhiCommandBuffer* buffer, QRhiRenderTarget* renderTarget)
{
	QRhiResourceUpdateBatch* batch = mRhi->nextResourceUpdateBatch();

	tryResetSkyBox(batch);

	for (auto& it : mProxyUploadList)
		it->uploadResource(batch);
	mProxyUploadList.clear();

	buffer->resourceUpdate(batch);
	for (auto& id : mPrimitiveProxyMap.keys()) {
		auto& proxy = mPrimitiveProxyMap[id];
		if (proxy->mComponent->bNeedResetProxy) {
			resetPrimitiveProxy(std::dynamic_pointer_cast<QPrimitiveComponent>(proxy->mComponent));
		}
	}

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
	if (mScene->mCamera); {
		return mScene->mCamera->getMatrixClip();
	}
	return QMatrix4x4();
}

QMatrix4x4 QSceneRenderer::getVP()
{
	if (mScene->mCamera); {
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
	std::shared_ptr<QRhiProxy> proxy = createPrimitiveProxy(primitive);
	mPrimitiveProxyMap[primitive->componentId()] = proxy;
	primitive->bNeedResetProxy = false;
	proxy->recreateResource();
	proxy->recreatePipeline();
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

void QSceneRenderer::tryResetSkyBox(QRhiResourceUpdateBatch* batch)
{
	if (!mSkyBoxProxy && mScene->getSkyBox()) {
		mSkyBoxProxy = createSkyBoxProxy(mScene->getSkyBox());
		mSkyBoxProxy->mRhi = mRhi;
		mSkyBoxProxy->mRenderer = this;
		mSkyBoxProxy->mComponent = mScene->getSkyBox();
		mSkyBoxProxy->recreateResource();
		mSkyBoxProxy->recreatePipeline();
		mSkyBoxProxy->uploadResource(batch);
	}
}

std::shared_ptr<QRhiProxy> QSceneRenderer::createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component)
{
	if (!component)
		return nullptr;
	std::shared_ptr<QRhiProxy> proxy;
	switch (component->type())
	{
	default:
		break;
	case QSceneComponent::None:
		return nullptr;
		break;
	case QSceneComponent::Shape:
		proxy = createShapeProxy(std::dynamic_pointer_cast<QShapeComponent>(component));
		break;
	case QSceneComponent::StaticMesh:
		proxy = createStaticMeshProxy(std::dynamic_pointer_cast<QStaticMeshComponent>(component));
		break;
	case QSceneComponent::SkeletonMesh:
		proxy = createSkeletonMeshProxy(std::dynamic_pointer_cast<QSkeletonMeshComponent>(component));
		break;
	case QSceneComponent::Particle:
		proxy = createParticleProxy(std::dynamic_pointer_cast<QParticleComponent>(component));
		break;
	}
	proxy->mRhi = mRhi;
	proxy->mRenderer = this;
	proxy->mComponent = component;
	return std::dynamic_pointer_cast<QRhiProxy>(proxy);
}

void QSceneRenderer::resetPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component)
{
	std::shared_ptr<QRhiProxy> newProxy = createPrimitiveProxy(component);
	newProxy->mComponent = component;
	newProxy->mRenderer = this;
	newProxy->mRhi = this->mRhi;
	newProxy->recreateResource();
	newProxy->recreatePipeline();
	mPrimitiveProxyMap[component->componentId()] = newProxy;
	mProxyUploadList << newProxy;
}