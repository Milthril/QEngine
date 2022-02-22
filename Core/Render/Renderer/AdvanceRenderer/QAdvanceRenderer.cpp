#include "QAdvanceRenderer.h"

std::shared_ptr<QRhiGraphicsPipeline> QAdvanceRenderer::createShapePipline(std::shared_ptr<QShapeComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiGraphicsPipeline> QAdvanceRenderer::createStaticMeshPipline(std::shared_ptr<QStaticMeshComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiGraphicsPipeline> QAdvanceRenderer::createSkeletonMeshPipline(std::shared_ptr<QSkeletonMeshComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiGraphicsPipeline> QAdvanceRenderer::createText2DPipline(std::shared_ptr<QText2DComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiGraphicsPipeline> QAdvanceRenderer::createParticlePipline(std::shared_ptr<QParticleComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiGraphicsPipeline> QAdvanceRenderer::createSkyBoxPipline(std::shared_ptr<QSkyBoxComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QAdvanceRenderer::createRhiProxy(std::shared_ptr<QSceneComponent> component)
{
	throw std::logic_error("The method or operation is not implemented.");
}