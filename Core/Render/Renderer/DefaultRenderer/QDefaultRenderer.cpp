#include "QDefaultRenderer.h"
#include "Render\Scene\Component\QShapeComponent.h"

#include "QtTest\qtestcase.h"
#include "QDefaultProxyShape.h"
#include "QDefaultProxyParticle.h"

QDefaultRenderer::QDefaultRenderer(std::shared_ptr<QRhi> rhi, int sampleCount, QRhiSPtr<QRhiRenderPassDescriptor> renderPassDescriptor)
	:QSceneRenderer(rhi, sampleCount, renderPassDescriptor)
{
	mBloomPainter.reset(new QBloomPainter(rhi));
}

void QDefaultRenderer::render(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget)
{
	QSize size = renderTarget->pixelSize();
	createOrResizeRenderTarget(size);
	for (auto& it : mProxyMap) {
		it->updatePrePass(cmdBuffer);
	}
	QRhiResourceUpdateBatch* batch = mRhi->nextResourceUpdateBatch();
	for (auto& it : mProxyMap) {
		it->updateResource(batch);
	}
	cmdBuffer->beginPass(mRT.renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 }, batch);
	QRhiViewport viewport(0, 0, size.width(), size.height());
	for (auto& proxy : mProxyMap) {
		proxy->drawInPass(cmdBuffer, viewport);
	}
	cmdBuffer->endPass();
	mBloomPainter->drawCommand(cmdBuffer, mRT.colorAttachment, renderTarget);
}

QRhiSPtr<QRhiRenderPassDescriptor> QDefaultRenderer::getRenderPassDescriptor() const
{
	return mRT.renderPassDesc;
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

std::shared_ptr<QRhiProxy> QDefaultRenderer::createParticleProxy(std::shared_ptr<QParticleComponent> comp)
{
	return std::make_shared<QDefaultProxyParticle>(comp);
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void QDefaultRenderer::createOrResizeRenderTarget(QSize size)
{
	if (mRT.colorAttachment && mRT.colorAttachment->pixelSize() == size)
		return;
	mRT.colorAttachment.reset(mRhi->newTexture(QRhiTexture::RGBA32F, size, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.colorAttachment->create();
	QRhiColorAttachment colorAttachment;
	if (getSampleCount() > 1) {
		mRT.msaaBuffer.reset(mRhi->newRenderBuffer(QRhiRenderBuffer::Color, size, getSampleCount(), {}, QRhiTexture::RGBA32F));
		mRT.msaaBuffer->create();
		colorAttachment.setRenderBuffer(mRT.msaaBuffer.get());
		colorAttachment.setResolveTexture(mRT.colorAttachment.get());
	}
	else {
		colorAttachment.setTexture(mRT.colorAttachment.get());
	}
	mRT.depthStencil.reset(mRhi->newRenderBuffer(QRhiRenderBuffer::DepthStencil, size, getSampleCount()));
	mRT.depthStencil->create();
	QRhiTextureRenderTargetDescription RTDesc;
	RTDesc.setColorAttachments({ colorAttachment });
	RTDesc.setDepthStencilBuffer(mRT.depthStencil.get());
	mRT.renderTarget.reset(mRhi->newTextureRenderTarget(RTDesc));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
}