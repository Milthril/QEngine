#include "QSceneRenderer.h"
#include "Render/Scene/Component/QPrimitiveComponent.h"
#include "Render/Scene/Component/QStaticMeshComponent .h"
#include "Render/Scene/Component/QShapeComponent.h"
#include "Render/Scene/Component/QSkeletonMeshComponent.h"
#include "Render/Scene/Component/QLightComponent.h"
#include "Render/Scene/Component/QSkyBoxComponent.h"
#include "private/qshaderbaker_p.h"

QSceneRenderer::QSceneRenderer(std::shared_ptr<QRhi> rhi, int sampleCount, QRhiSPtr<QRhiRenderPassDescriptor> renderPassDescriptor)
	: mRhi(rhi)
	, mSampleCount(sampleCount)
	, mRootRenderPassDescriptor(renderPassDescriptor)
{
	mClipMatrix = mRhi->clipSpaceCorrMatrix();
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
	QMatrix4x4 ClipMatrix = mRhi->clipSpaceCorrMatrix();
	ClipMatrix.perspective(45, size.width() / (float)size.height(), 0.01f, 1000.0f);
	setClipMatrix(ClipMatrix);
}

void QSceneRenderer::renderInternal(QRhiCommandBuffer* buffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch)
{
	for (auto& it : mProxyUploadList) {
		it->uploadResource(batch);
	}
	mProxyUploadList.clear();
	for (auto& id : mProxyMap.keys()) {
		auto& proxy = mProxyMap[id];
		if (proxy->mComponent->bNeedResetProxy) {
			resetPrimitiveProxy(std::dynamic_pointer_cast<QPrimitiveComponent>(proxy->mComponent));
		}
	}
	render(buffer, renderTarget, batch);
}

QMatrix4x4 QSceneRenderer::getViewMatrix()
{
	if (mScene->mCamera) {
		return QMatrix4x4();
	}
	QMatrix4x4 matrix;
	matrix.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
	return matrix;
}

QMatrix4x4 QSceneRenderer::getClipMatrix() const
{
	return mClipMatrix;
}

QMatrix4x4 QSceneRenderer::getVP()
{
	return getClipMatrix() * getViewMatrix();
}

void QSceneRenderer::setClipMatrix(QMatrix4x4 val)
{
	mClipMatrix = val;
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
	proxy->mComponent = primitive;
	proxy->mRenderer = this;
	proxy->mRhi = this->mRhi;
	mProxyMap[primitive->componentId()] = proxy;
	primitive->bNeedResetProxy = false;
	proxy->recreateResource();
	mProxyUploadList << proxy;
}

void QSceneRenderer::onPrimitiveRemoved(std::shared_ptr<QPrimitiveComponent> primitive)
{
	mProxyMap.remove(primitive->componentId());
}

void QSceneRenderer::onLightChanged()
{
}

std::shared_ptr<QRhiProxy> QSceneRenderer::createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component)
{
	switch (component->type())
	{
	default:
		break;
	case QSceneComponent::None:
		return nullptr;
		break;
	case QSceneComponent::Shape:
		return createShapeProxy(std::dynamic_pointer_cast<QShapeComponent>(component));
		break;
	case QSceneComponent::StaticMesh:
		return createStaticMeshProxy(std::dynamic_pointer_cast<QStaticMeshComponent>(component));
		break;
	case QSceneComponent::SkeletonMesh:
		return createSkeletonMeshProxy(std::dynamic_pointer_cast<QSkeletonMeshComponent>(component));
		break;
	case QSceneComponent::Particle:
		return createParticleProxy(std::dynamic_pointer_cast<QParticleComponent>(component));
		break;
	case QSceneComponent::SkyBox:
		return createSkyBoxProxy(std::dynamic_pointer_cast<QSkyBoxComponent>(component));
		break;
	}
	return nullptr;
}

void QSceneRenderer::resetPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component)
{
	std::shared_ptr<QRhiProxy> newProxy = createPrimitiveProxy(component);
	newProxy->mComponent = component;
	newProxy->mRenderer = this;
	newProxy->mRhi = this->mRhi;
	newProxy->recreateResource();
	mProxyMap[component->componentId()] = newProxy;
	mProxyUploadList << newProxy;
}