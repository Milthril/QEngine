#include "DeferRenderPass.h"
#include "QEngineCoreApplication.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"
#include "ECS/System/RenderSystem/Renderer/QRenderer.h"
#include "ECS/System/RenderSystem/RHI/IRenderable.h"

void DeferRenderPass::compile() {		//创建默认的RT
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

	mRT.atPosition.reset(RHI->newTexture(QRhiTexture::RGBA32F, frameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.atPosition->create();
	mBlendStates << blendState;
	mColorAttachmentList << QRhiColorAttachment(mRT.atPosition.get());

	mRT.atNormal_Metalness.reset(RHI->newTexture(QRhiTexture::RGBA32F, frameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.atNormal_Metalness->create();
	mBlendStates << blendState;
	mColorAttachmentList << QRhiColorAttachment(mRT.atNormal_Metalness.get());

	mRT.atTangent_Roughness.reset(RHI->newTexture(QRhiTexture::RGBA32F, frameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.atTangent_Roughness->create();
	mBlendStates << blendState;
	mColorAttachmentList << QRhiColorAttachment(mRT.atTangent_Roughness.get());

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
	mRT.atDepthStencil.reset(RHI->newRenderBuffer(QRhiRenderBuffer::DepthStencil, frameSize, 1, {},QRhiTexture::Format::D24S8));
	mRT.atDepthStencil->create();
	RTDesc.setDepthStencilBuffer(mRT.atDepthStencil.get());
	mRT.renderTarget.reset(RHI->newTextureRenderTarget(RTDesc));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
}

DeferRenderPass::DeferRenderPass() {
}

QRhiTexture* DeferRenderPass::getOutputTexture(int slot /*= 0*/) {
	switch ((OutputTextureSlot)slot) {
	case BaseColor: return mRT.atBaseColor.get();
	case Position: return mRT.atPosition.get();
	case NormalMetalness: return mRT.atNormal_Metalness.get();
	case TangentRoughness: return mRT.atTangent_Roughness.get();
	case DebugId: return mRT.atDebugId.get();
	}
	return nullptr;
}

QVector<QRhiGraphicsPipeline::TargetBlend> DeferRenderPass::getBlendStates()
{
	return mBlendStates;
}

QRhiRenderTarget* DeferRenderPass::getRenderTarget()
{
	return mRT.renderTarget.get();
}

void DeferRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {
	for (auto& item : mRenderer->getRenderableItemList()) {
		item->updatePrePass(cmdBuffer);
	}
	QRhiResourceUpdateBatch* resUpdateBatch = RHI->nextResourceUpdateBatch();
	for (auto& item : mRenderer->getRenderableItemList()) {
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
	for (auto& item : mRenderer->getRenderableItemList()) {
		if (item->bNeedRecreatePipeline.receive()) {
			item->recreatePipeline();
		}
		item->renderInPass(cmdBuffer, viewport);
	}
	cmdBuffer->endPass();
}
