#include "QAdvanceRenderer.h"
#include "Render\Scene\Component\QShapeComponent.h"
#include "QtTest\qtestcase.h"

std::shared_ptr<QRhiProxy> QAdvanceRenderer::createShapeProxy(std::shared_ptr<QShapeComponent> shape)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QAdvanceRenderer::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QAdvanceRenderer::createSkeletonMeshProxy(std::shared_ptr<QSkeletonMeshComponent>)
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