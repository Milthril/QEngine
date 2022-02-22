#include "QSceneRenderer.h"
#include "..\Scene\Component\QPrimitiveComponent.h"
#include "..\Scene\Component\QStaticMeshComponent .h"

std::shared_ptr<QRhiGraphicsPipeline> QSceneRenderer::createPrimitivePipline(std::shared_ptr<QPrimitiveComponent> component)
{
	switch (component->type())
	{
	default:
		break;
	case QSceneComponent::None:
		return nullptr;
		break;
	case QSceneComponent::Shape:
		return createShapePipline(std::dynamic_pointer_cast<QShapeComponent>(visible));
		break;
	case QSceneComponent::StaticMesh:
		return createStaticMeshPipline(std::dynamic_pointer_cast<QStaticMeshComponent>(visible));
		break;
	case QSceneComponent::SkeletonMesh:
		return createSkeletonMeshPipline(std::dynamic_pointer_cast<QSkeletonMeshComponent>(visible));
		break;
	case QSceneComponent::Text2D:
		return createText2DPipline(std::dynamic_pointer_cast<QText2DComponent>(visible));
		break;
	case QSceneComponent::Particle:
		return createParticlePipline(std::dynamic_pointer_cast<QParticleComponent>(visible));
		break;
	case QSceneComponent::SkyBox:
		return createSkyBoxPipline(std::dynamic_pointer_cast<QSkyBoxComponent>(visible));
		break;
	}
}
