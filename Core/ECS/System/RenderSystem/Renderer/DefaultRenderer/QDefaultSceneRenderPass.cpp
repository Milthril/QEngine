#include "QDefaultSceneRenderPass.h"
#include "QEngineCoreApplication.h"
#include "QDefaultRenderer.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"


void QDefaultSceneRenderPass::compile() {		//创建默认的RT
	if (mRT.colorAttachment && mRT.colorAttachment->pixelSize() == mSceneFrameSize)
		return;
	mRT.colorAttachment.reset(RHI->newTexture(QRhiTexture::RGBA32F, mSceneFrameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.colorAttachment->create();
	QVector<QRhiColorAttachment> colorAttachments;
	QRhiColorAttachment colorAttachment;
	if (mSampleCount > 1) {
		mRT.msaaBuffer.reset(RHI->newRenderBuffer(QRhiRenderBuffer::Color, mSceneFrameSize, mSampleCount, {}, QRhiTexture::RGBA32F));
		mRT.msaaBuffer->create();
		colorAttachment.setRenderBuffer(mRT.msaaBuffer.get());
		colorAttachment.setResolveTexture(mRT.colorAttachment.get());
	}
	else {
		colorAttachment.setTexture(mRT.colorAttachment.get());
	}
	colorAttachments << colorAttachment;
	mBlendStates.clear();
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = false;
	mBlendStates << blendState;
	if (QRenderSystem::instance()->isEnableDebug()) {
		mBlendStates << blendState;
		QRhiColorAttachment debugAttachment;
		mRT.debugTexture.reset(RHI->newTexture(QRhiTexture::RGBA8, mSceneFrameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
		mRT.debugTexture->create();
		if (mSampleCount > 1) {
			mRT.debugMsaaBuffer.reset(RHI->newRenderBuffer(QRhiRenderBuffer::Color, mSceneFrameSize, mSampleCount, {}, QRhiTexture::RGBA8));
			mRT.debugMsaaBuffer->create();
			debugAttachment.setRenderBuffer(mRT.debugMsaaBuffer.get());
			debugAttachment.setResolveTexture(mRT.debugTexture.get());
		}
		else {
			debugAttachment.setTexture(mRT.debugTexture.get());
		}
		colorAttachments << debugAttachment;
	}
	QRhiTextureRenderTargetDescription RTDesc;
	RTDesc.setColorAttachments(colorAttachments.begin(), colorAttachments.end());
	mRT.depthStencil.reset(RHI->newRenderBuffer(QRhiRenderBuffer::DepthStencil, mSceneFrameSize, mSampleCount));
	mRT.depthStencil->create();
	RTDesc.setDepthStencilBuffer(mRT.depthStencil.get());
	mRT.renderTarget.reset(RHI->newTextureRenderTarget(RTDesc));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
}

QDefaultSceneRenderPass::QDefaultSceneRenderPass() {

}



QRhiTexture* QDefaultSceneRenderPass::getOutputTexture()
{
	return mRT.colorAttachment.get();
}

QRhiTexture* QDefaultSceneRenderPass::getDebugTexutre() {
	return mRT.debugTexture.get();
}

QVector<QRhiGraphicsPipeline::TargetBlend> QDefaultSceneRenderPass::getBlendStates()
{
	return mBlendStates;
}

QRhiRenderTarget* QDefaultSceneRenderPass::getRenderTarget()
{
	return mRT.renderTarget.get();
}