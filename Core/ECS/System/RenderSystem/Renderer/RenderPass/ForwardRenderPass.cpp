#include "ForwardRenderPass.h"
#include "QEngineCoreApplication.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"
#include "ECS/System/RenderSystem/Renderer/QRenderer.h"
#include "ECS/System/RenderSystem/RHI/IRenderable.h"

void ForwardRenderPass::compile() {		//创建默认的RT

	QSize frameSize = mRenderer->getFrameSize();
	if (mRT.atBaseColor && mRT.atBaseColor->pixelSize() == frameSize)
		return;

	QVector<QRhiColorAttachment> mColorAttachmentList;
	mBlendStates.clear();
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = false;

	mRT.atBaseColor.reset(RHI->newTexture(QRhiTexture::RGBA32F, frameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.atBaseColor->create();
	mBlendStates << blendState;
	mColorAttachmentList << QRhiColorAttachment(mRT.atBaseColor.get());

	if (TheRenderSystem->isEnableDebug()) {
		mBlendStates << blendState;
		QRhiColorAttachment debugAttachment;
		mRT.atDebugId.reset(RHI->newTexture(QRhiTexture::RGBA8, frameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
		mRT.atDebugId->create();
		debugAttachment.setTexture(mRT.atDebugId.get());
		mColorAttachmentList << debugAttachment;
	}

	QRhiTextureRenderTargetDescription RTDesc;
	RTDesc.setColorAttachments(mColorAttachmentList.begin(), mColorAttachmentList.end());
	mRT.atDepthStencil.reset(RHI->newRenderBuffer(QRhiRenderBuffer::DepthStencil, frameSize, 1, {},QRhiTexture::Format::D32F));
	mRT.atDepthStencil->create();
	RTDesc.setDepthStencilBuffer(mRT.atDepthStencil.get());
	mRT.renderTarget.reset(RHI->newTextureRenderTarget(RTDesc));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
}

ForwardRenderPass::ForwardRenderPass() {
}

void ForwardRenderPass::setupDeferColorTexture(QRhiTexture* texture) {
	mDeferColorResult = texture;
}

void ForwardRenderPass::setupDeferDepthTexture(QRhiTexture* texture) {
	mDeferDepthResult = texture;
}

QRhiTexture* ForwardRenderPass::getOutputTexture(int slot /*= 0*/) {
	switch ((OutputTextureSlot)slot) {
	case Output: return mRT.atBaseColor.get();
	}
	return nullptr;
}

QVector<QRhiGraphicsPipeline::TargetBlend> ForwardRenderPass::getBlendStates()
{
	return mBlendStates;
}

QRhiRenderTarget* ForwardRenderPass::getRenderTarget()
{
	return mRT.renderTarget.get();
}

void ForwardRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {
	const auto& forwardList = mRenderer->getForwardItemList();
	for (auto& item : forwardList) {
		item->updatePrePass(cmdBuffer);
	}
	QRhiResourceUpdateBatch* resUpdateBatch = RHI->nextResourceUpdateBatch();
	for (auto& item : forwardList) {
		if (item->bNeedRecreateResource.receive()) {
			item->recreateResource();
			item->uploadResource(resUpdateBatch);
		}
		item->updateResourcePrePass(resUpdateBatch);
		if (item->bNeedRecreatePipeline.receive()) {
			item->recreatePipeline();
		}
	}
	cmdBuffer->beginPass(getRenderTarget(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 }, resUpdateBatch);
	QRhiViewport viewport(0, 0, getRenderTarget()->pixelSize().width(), getRenderTarget()->pixelSize().height());
	for (auto& item : forwardList) {
		if (item->bNeedRecreatePipeline.receive()) {
			item->recreatePipeline();
		}
		item->renderInPass(cmdBuffer, viewport);
	}
	cmdBuffer->endPass();
}
