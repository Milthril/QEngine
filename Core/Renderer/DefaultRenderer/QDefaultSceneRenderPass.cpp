#include "QDefaultSceneRenderPass.h"
#include "QEngine.h"
#include "Renderer/IRhiProxy.h"
#include "Scene/Component/QPrimitiveComponent.h"
#include "QDefaultProxyStaticMesh.h"
#include "QDefaultProxySkeletonModel.h"
#include "QDefaultProxyParticle.h"
#include "QDefaultProxySkyBox.h"
#include "QDefaultRenderer.h"

QDefaultSceneRenderPass::QDefaultSceneRenderPass(std::shared_ptr<QScene> scene)
	:ISceneRenderPass(scene)
{
}

std::shared_ptr<IRhiProxy> QDefaultSceneRenderPass::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent> comp) {
	return std::make_shared<QDefaultProxyStaticMesh>(comp);
}

std::shared_ptr<IRhiProxy> QDefaultSceneRenderPass::createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent> comp) {
	return std::make_shared<QDefaultProxySkeletonModel>(comp);
}

std::shared_ptr<IRhiProxy> QDefaultSceneRenderPass::createParticleProxy(std::shared_ptr<QParticleComponent> comp) {
	return std::make_shared<QDefaultProxyParticle>(comp);
}

std::shared_ptr<IRhiProxy> QDefaultSceneRenderPass::createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent> comp) {
	return std::make_shared<QDefaultProxySkyBox>(comp);
}

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

	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = false;
	mBlendStates << blendState;
	if (mEnableOutputDebugId) {
		mBlendStates << blendState;
		QRhiColorAttachment debugAttachment;
		mRT.debugTexture.reset(RHI->newTexture(QRhiTexture::RGBA32F, mSceneFrameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
		mRT.debugTexture->create();
		if (mSampleCount > 1) {
			mRT.debugMsaaBuffer.reset(RHI->newRenderBuffer(QRhiRenderBuffer::Color, mSceneFrameSize, mSampleCount, {}, QRhiTexture::RGBA32F));
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
	RTDesc.setColorAttachments(colorAttachments.begin(), colorAttachments.end());
	mRT.depthStencil.reset(RHI->newRenderBuffer(QRhiRenderBuffer::DepthStencil, mSceneFrameSize, mSampleCount));
	mRT.depthStencil->create();
	RTDesc.setDepthStencilBuffer(mRT.depthStencil.get());
	mRT.renderTarget.reset(RHI->newTextureRenderTarget(RTDesc));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
}

void QDefaultSceneRenderPass::setupSceneFrameSize(QSize size)
{
	mSceneFrameSize = size;
}

void QDefaultSceneRenderPass::setupSampleCount(int count)
{
	mSampleCount = count;
}

QRhiTexture* QDefaultSceneRenderPass::getOutputTexture()
{
	return mRT.colorAttachment.get();
}

QVector<QRhiGraphicsPipeline::TargetBlend> QDefaultSceneRenderPass::getBlendStates()
{
	return mBlendStates;
}

QRhiRenderTarget* QDefaultSceneRenderPass::getRenderTarget()
{
	return mRT.renderTarget.get();
}