#include "QDefaultRenderer.h"
#include "Render\Scene\Component\QShapeComponent.h"
#include "private\qshaderbaker_p.h"
#include "QtTest\qtestcase.h"
#include "QDefaultProxyShape.h"

QDefaultRenderer::QDefaultRenderer(std::shared_ptr<QRhi> rhi, int sampleCount, std::shared_ptr<QRhiRenderPassDescriptor> renderPassDescriptor)
	:QSceneRenderer(rhi, sampleCount, renderPassDescriptor)
{
	mFSPainter.reset(new QFullSceneTexturePainter(rhi));
	createOrResizeRenderTarget({ 10,10 });
	mFSPainter->initRhiResource(mRT.renderPassDesc, sampleCount);
}

void QDefaultRenderer::render(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch)
{
	QSize size = renderTarget->pixelSize();
	createOrResizeRenderTarget(size);
	for (auto it : mProxyMap) {
		it->updateResource(batch);
	}
	cmdBuffer->beginPass(mRT.renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 }, batch);
	QRhiViewport viewport(0, 0, size.width(), size.height());
	for (auto& proxy : mProxyMap) {
		cmdBuffer->setGraphicsPipeline(proxy->mPipeline.get());
		cmdBuffer->setViewport(viewport);
		proxy->draw(cmdBuffer);
	}
	cmdBuffer->endPass();
	mFSPainter->drawCommand(cmdBuffer, renderTarget);
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createShapeProxy(std::shared_ptr<QShapeComponent> shape)
{
	return std::make_shared<QDefaultProxyShape>(shape);
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createSkeletonMeshProxy(std::shared_ptr<QSkeletonMeshComponent>)
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

void QDefaultRenderer::createOrResizeRenderTarget(QSize size)
{
	if (mRT.colorAttachment && mRT.colorAttachment->pixelSize() == size)
		return;
	mRT.colorAttachment.reset(mRhi->newTexture(QRhiTexture::RGBA8, size, getSampleCount(), QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.colorAttachment->create();
	mRT.depthStencil.reset(mRhi->newRenderBuffer(QRhiRenderBuffer::DepthStencil, size, getSampleCount()));
	mRT.depthStencil->create();
	mRT.renderTarget.reset(mRhi->newTextureRenderTarget({ mRT.colorAttachment.get(),mRT.depthStencil.get() }));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
	mFSPainter->updateTexture(mRT.colorAttachment);
}