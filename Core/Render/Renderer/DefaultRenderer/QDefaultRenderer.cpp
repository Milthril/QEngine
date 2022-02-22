#include "QDefaultRenderer.h"
#include "Render\Scene\Component\QShapeComponent.h"
#include "private\qshaderbaker_p.h"
#include "QtTest\qtestcase.h"
#include "QDefaultProxyShape.h"

void QDefaultRenderer::render(QRhiCommandBuffer* buffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch)
{
	QSize size = renderTarget->pixelSize();
	QMatrix4x4 VP = getClipMatrix()* getViewMatrix();
	for (auto it : mProxyMap) {
		it->updateResource(batch, VP);
	}
	buffer->beginPass(renderTarget, QColor::fromRgbF(0.4f, 0.7f, 0.0f, 1.0f), { 1.0f, 0 }, batch);

	QRhiViewport viewport(0, 0, size.width(), size.height());

	for (auto proxy : mProxyMap) {
		buffer->setGraphicsPipeline(proxy->mPipeline.get());
		buffer->setViewport(viewport);
		proxy->draw(buffer);
	}
	buffer->endPass();
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createShapeProxy(std::shared_ptr<QShapeComponent> shape)
{
	return std::make_shared<QDefaultProxyShape>(this, shape);
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createSkeletonMeshProxy(std::shared_ptr<QSkeletonMeshComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createText2DProxy(std::shared_ptr<QText2DComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createParticleProxy(std::shared_ptr<QParticleComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}