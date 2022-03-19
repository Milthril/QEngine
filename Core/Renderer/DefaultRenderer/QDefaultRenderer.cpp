#include "QDefaultRenderer.h"
#include "Scene\Component\StaticMesh\QStaticMeshComponent.h"
#include "QDefaultProxyStaticMesh.h"
#include "QDefaultProxyParticle.h"
#include "QDefaultProxySkyBox.h"
#include "QEngine.h"
#include "QDefaultProxySkeletonModel.h"

QDefaultRenderer::QDefaultRenderer()
{
	mReadReult.completed = [this]() {
		const uchar* p = reinterpret_cast<const uchar*>(mReadReult.data.constData());
		int offset = (mReadReult.pixelSize.width() * mReadPoint.y() + mReadPoint.x()) * 4;
		uint32_t id = p[offset] + p[offset + 1] * 256 + p[offset + 2] * 256 * 256 + p[offset + 3] * 256 * 256 * 256;
		mReadPoint = { 0,0 };
		Q_EMIT readBackCompId(id);
	};
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
	if (debugEnabled() &&!mReadPoint.isNull()) {
		batch = RHI->nextResourceUpdateBatch();
		batch->readBackTexture(mReadDesc, &mReadReult);
		cmdBuffer->resourceUpdate(batch);
		RHI->finish();
	}

	mBloomPainter->drawCommand(cmdBuffer, mRT.colorAttachment, renderTarget);
}

QRhiSPtr<QRhiRenderPassDescriptor> QDefaultRenderer::getRenderPassDescriptor() const
{
	return mRT.renderPassDesc;
}

QVector<QRhiGraphicsPipeline::TargetBlend> QDefaultRenderer::getDefaultBlends()
{
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = false;
	if (debugEnabled()) {
		return { blendState,blendState };
	}
	return { blendState };
}

void QDefaultRenderer::requestReadbackCompId(const QPoint& screenPt)
{
	mReadPoint = screenPt;
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent> comp)
{
	return std::make_shared<QDefaultProxyStaticMesh>(comp);
}

std::shared_ptr<QRhiProxy> QDefaultRenderer::createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent> comp)
{
	return std::make_shared<QDefaultProxySkeletonModel>(comp);
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
	QVector<QRhiColorAttachment> colorAttachments;
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
	colorAttachments << colorAttachment;

	if (debugEnabled()) {
		QRhiColorAttachment debugAttachment;
		mRT.debugTexture.reset(RHI->newTexture(QRhiTexture::RGBA8, size,1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
		mRT.debugTexture->create();
		if (getSampleCount() > 1) {
			mRT.debugMsaaBuffer.reset(RHI->newRenderBuffer(QRhiRenderBuffer::Color, size, getSampleCount(), {}, QRhiTexture::RGBA8));
			mRT.debugMsaaBuffer->create();
			debugAttachment.setRenderBuffer(mRT.debugMsaaBuffer.get());
			debugAttachment.setResolveTexture(mRT.debugTexture.get());
		}
		else {
			debugAttachment.setTexture(mRT.debugTexture.get());
		}
		colorAttachments << debugAttachment;
		mReadDesc.setTexture(mRT.debugTexture.get());
	}
	QRhiTextureRenderTargetDescription RTDesc;
	RTDesc.setColorAttachments(colorAttachments.begin(),colorAttachments.end());
	mRT.depthStencil.reset(RHI->newRenderBuffer(QRhiRenderBuffer::DepthStencil, size, getSampleCount()));
	mRT.depthStencil->create();
	RTDesc.setDepthStencilBuffer(mRT.depthStencil.get());
	mRT.renderTarget.reset(RHI->newTextureRenderTarget(RTDesc));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
}