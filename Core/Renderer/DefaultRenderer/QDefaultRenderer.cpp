#include "QDefaultRenderer.h"
#include "Scene\Component\StaticMesh\QStaticMeshComponent.h"
#include "QDefaultProxyStaticMesh.h"
#include "QDefaultProxyParticle.h"
#include "QDefaultProxySkyBox.h"
#include "QEngine.h"

QDefaultRenderer::QDefaultRenderer(int sampleCount, QRhiSPtr<QRhiRenderPassDescriptor> renderPassDescriptor)
	:QSceneRenderer(sampleCount, renderPassDescriptor)
{
}

void QDefaultRenderer::render(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget)
{
	QSize size = renderTarget->pixelSize();
	createOrResizeRenderTarget(size);
	for (auto& it : mPrimitiveProxyMap) {
		it->updatePrePass(cmdBuffer);
	}
	QRhiResourceUpdateBatch* batch = RHI->nextResourceUpdateBatch();
	for (auto& it : mPrimitiveProxyMap) {
		it->updateResource(batch);
	}
	if (mSkyBoxProxy) {
		mSkyBoxProxy->updateResource(batch);
	}
	cmdBuffer->beginPass(mRT.renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 }, batch);

	QRhiViewport viewport(0, 0, size.width(), size.height());
	for (auto& proxy : mPrimitiveProxyMap) {
		proxy->drawInPass(cmdBuffer, viewport);
	}
	if (mSkyBoxProxy) {
		mSkyBoxProxy->drawInPass(cmdBuffer, viewport);
	}
	cmdBuffer->endPass();
	mBloomPainter->drawCommand(cmdBuffer, mRT.colorAttachment, renderTarget);
}

QRhiSPtr<QRhiRenderPassDescriptor> QDefaultRenderer::getRenderPassDescriptor() const
{
	return mRT.renderPassDesc;
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent> comp)
{
	return std::make_shared<QDefaultProxyStaticMesh>(comp);
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createSkeletonMeshProxy(std::shared_ptr<QSkeletonMeshComponent>)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createParticleProxy(std::shared_ptr<QParticleComponent> comp)
{
	return std::make_shared<QDefaultProxyParticle>(comp);
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent> comp)
{
	return std::make_shared<QDefaultProxySkyBox>(comp);
}

void QDefaultRenderer::createOrResizeRenderTarget(QSize size)
{
	if (mRT.colorAttachment && mRT.colorAttachment->pixelSize() == size)
		return;
	mRT.colorAttachment.reset(RHI->newTexture(QRhiTexture::RGBA32F, size, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.colorAttachment->create();
	QRhiColorAttachment colorAttachment;
	if (getSampleCount() > 1) {
		mRT.msaaBuffer.reset(RHI->newRenderBuffer(QRhiRenderBuffer::Color, size, getSampleCount(), {}, QRhiTexture::RGBA32F));
		mRT.msaaBuffer->create();
		colorAttachment.setRenderBuffer(mRT.msaaBuffer.get());
		colorAttachment.setResolveTexture(mRT.colorAttachment.get());
	}
	else {
		colorAttachment.setTexture(mRT.colorAttachment.get());
	}
	mRT.depthStencil.reset(RHI->newRenderBuffer(QRhiRenderBuffer::DepthStencil, size, getSampleCount()));
	mRT.depthStencil->create();
	QRhiTextureRenderTargetDescription RTDesc;
	RTDesc.setColorAttachments({ colorAttachment });
	RTDesc.setDepthStencilBuffer(mRT.depthStencil.get());
	mRT.renderTarget.reset(RHI->newTextureRenderTarget(RTDesc));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
}