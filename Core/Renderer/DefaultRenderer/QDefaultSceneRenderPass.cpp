#include "QDefaultSceneRenderPass.h"
#include "QEngine.h"
#include "Renderer/ISceneComponentRenderProxy.h"
#include "Scene/Component/QPrimitiveComponent.h"
#include "QDefaultProxyStaticMesh.h"
#include "QDefaultProxySkeletonModel.h"
#include "QDefaultProxyParticle.h"
#include "QDefaultProxySkyBox.h"
#include "QDefaultRenderer.h"

QDefaultSceneRenderPass::QDefaultSceneRenderPass(QDefaultRenderer* renderer)
	: mRenderer(renderer)
{
}

std::shared_ptr<ISceneComponentRenderProxy> QDefaultSceneRenderPass::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent> comp) {
	return std::make_shared<QDefaultProxyStaticMesh>(comp);
}

std::shared_ptr<ISceneComponentRenderProxy> QDefaultSceneRenderPass::createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent> comp) {
	return std::make_shared<QDefaultProxySkeletonModel>(comp);
}

std::shared_ptr<ISceneComponentRenderProxy> QDefaultSceneRenderPass::createParticleProxy(std::shared_ptr<QParticleComponent> comp) {
	return std::make_shared<QDefaultProxyParticle>(comp);
}

std::shared_ptr<ISceneComponentRenderProxy> QDefaultSceneRenderPass::createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent> comp) {
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

	if (mEnableOutputDebugId) {
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

void QDefaultSceneRenderPass::execute()
{
	auto& primitiveList = mRenderer->getScene()->geyPrimitiveList();
	QRhiCommandBuffer* cmdBuffer;
	RHI->beginOffscreenFrame(&cmdBuffer);
	QRhiResourceUpdateBatch* resUpdateBatch = RHI->nextResourceUpdateBatch();
	for (int i = 0; i < primitiveList.size(); i++) {		//创建代理
		if (!mPrimitiveProxyMap.contains(primitiveList[i]->componentId())) {
			const auto& proxy = createPrimitiveProxy(primitiveList[i]);
			proxy->uploadResource(resUpdateBatch);
			mPrimitiveProxyMap[primitiveList[i]->componentId()] = proxy;
		}
	}
	cmdBuffer->resourceUpdate(resUpdateBatch);

	for (auto& proxy : mPrimitiveProxyMap) {
		proxy->updatePrePass(cmdBuffer);
	}
	resUpdateBatch = RHI->nextResourceUpdateBatch();
	for (auto& proxy : mPrimitiveProxyMap) {
		proxy->updateResource(resUpdateBatch);
	}
	cmdBuffer->beginPass(mRT.renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 }, resUpdateBatch);
	QRhiViewport viewport(0, 0, mSceneFrameSize.width(), mSceneFrameSize.height());
	for (auto& proxy : mPrimitiveProxyMap) {
		proxy->drawInPass(cmdBuffer, viewport);
	}
	cmdBuffer->endPass();
	RHI->endOffscreenFrame();
}

void QDefaultSceneRenderPass::setupSceneFrameSize(QSize size)
{
	mSceneFrameSize = size;
}

void QDefaultSceneRenderPass::setupSampleCount(int count)
{
	mSampleCount = count;
}

QRhiSPtr<QRhiTexture> QDefaultSceneRenderPass::getOutputTexture()
{
	return mRT.colorAttachment;
}

int QDefaultSceneRenderPass::getSampleCount()
{
	return mSampleCount;
}

QRhiRenderPassDescriptor* QDefaultSceneRenderPass::getRenderPassDescriptor()
{
	return mRT.renderPassDesc.get();
}

QVector<QRhiGraphicsPipeline::TargetBlend> QDefaultSceneRenderPass::getBlendStates()
{
	return {};
}