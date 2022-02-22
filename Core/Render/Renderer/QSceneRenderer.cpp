#include "QSceneRenderer.h"
#include "Render/Scene/Component/QPrimitiveComponent.h"
#include "Render/Scene/Component/QStaticMeshComponent .h"
#include "Render/Scene/Component/QShapeComponent.h"
#include "Render/Scene/Component/QSkeletonMeshComponent.h"
#include "Render/Scene/Component/QText2DComponent.h"
#include "Render/Scene/Component/QLightComponent.h"
#include "Render/Scene/Component/QSkyBoxComponent.h"

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

QMatrix4x4 QSceneRenderer::getViewMatrix()
{
	if (mScene->mCamera) {
		return QMatrix4x4();
	}
	QMatrix4x4 matrix;
	return matrix;
}

QMatrix4x4 QSceneRenderer::getClipMatrix() const
{
	return mClipMatrix;
}

void QSceneRenderer::setClipMatrix(QMatrix4x4 val)
{
	mClipMatrix = val;
}

void QSceneRenderer::onPrimitiveInserted(uint32_t index, std::shared_ptr<QPrimitiveComponent> primitive)
{
	mProxyMap[primitive->componentId()] = createPrimitiveProxy(primitive);
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
	case QSceneComponent::Text2D:
		return createText2DProxy(std::dynamic_pointer_cast<QText2DComponent>(component));
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