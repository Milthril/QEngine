#include "QAdvanceRenderer.h"

std::shared_ptr<QRhiProxy> QAdvanceRenderer::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent> shape)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QAdvanceRenderer::createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QAdvanceRenderer::createParticleProxy(std::shared_ptr<QParticleComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QAdvanceRenderer::createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}