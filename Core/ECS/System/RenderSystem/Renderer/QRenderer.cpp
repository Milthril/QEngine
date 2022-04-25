#include "QRenderer.h"
#include "QEngineCoreApplication.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\PixelSelectRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\BlurRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\BloomRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\SwapChainRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\LightingRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "RenderPass\DeferRenderPass.h"
#include "RenderPass\ForwardRenderPass.h"
#include "ECS\System\RenderSystem\RHI\IRenderable.h"

QRenderer::QRenderer(){}

void QRenderer::buildFrameGraph() {
	QFrameGraphBuilder builder;

	mDeferRenderPass = std::make_shared<DeferRenderPass>();
	mLightingRenderPass = std::make_shared<LightingRenderPass>();
	mForwardRenderPass = std::make_shared<ForwardRenderPass>();

	std::shared_ptr<PixelSelectRenderPass> bloomPixelSelectPass = std::make_shared<PixelSelectRenderPass>();
	std::shared_ptr<BlurRenderPass> bloomBlurPass = std::make_shared<BlurRenderPass>();
	std::shared_ptr<BloomMerageRenderPass> bloomMeragePass = std::make_shared<BloomMerageRenderPass>();
	std::shared_ptr<SwapChainRenderPass> swapChainPass = std::make_shared<SwapChainRenderPass>();

	//	ScenePass -> LightingPass ->ForwardPass -->BloomPixelSeletorPass -> BloomBlurPass
	//	                 |														 |
	//	                 |														 V
	//					 -----------------------------------------------> BloomMeragePass ----> Swapchain( Scene + ImguiDebugDraw )

	mFrameGraph = builder.begin(this)			
		->node("Defer", mDeferRenderPass,[]() {})
		->node("Lighting", mLightingRenderPass,
			   [self = mLightingRenderPass.get(), defer = mDeferRenderPass.get()]() {
					self->setupBaseColorTexutre(defer->getOutputTexture(DeferRenderPass::OutputTextureSlot::BaseColor));
					self->setupPositionTexutre(defer->getOutputTexture(DeferRenderPass::OutputTextureSlot::Position));
					self->setupNormalMetalnessTexutre(defer->getOutputTexture(DeferRenderPass::OutputTextureSlot::NormalMetalness));
					self->setupTangentRoughnessTexutre(defer->getOutputTexture(DeferRenderPass::OutputTextureSlot::TangentRoughness));
				})
		->dependency({ "Defer" })
		->node("Forward", mForwardRenderPass,
				[self = mForwardRenderPass.get(), lighting = mLightingRenderPass.get(), defer = mDeferRenderPass.get()]() {
					self->setupInputColorTexture(lighting->getOutputTexture(LightingRenderPass::OutputTextureSlot::LightingResult));
					self->setupInputDepthTexture(defer->getOutputTexture(DeferRenderPass::OutputTextureSlot::Depth));
					self->setupInputDebugIdTexture(defer->getOutputTexture(DeferRenderPass::OutputTextureSlot::DebugId));
				})
		->dependency({ "Lighting","Defer"})
		->node("BloomPixelSelector", bloomPixelSelectPass,
				[self = bloomPixelSelectPass.get(), forward = mForwardRenderPass.get()]() {
					self->setDownSamplerCount(4);
					self->setupSelectCode(R"(
						void main() {
							vec4 color = texture(uTexture, vUV);
							float value = max(max(color.r,color.g),color.b);
							outFragColor = (1-step(value,1.0f)) * color;
						}
						)");
					self->setupInputTexture(forward->getOutputTexture(ForwardRenderPass::OutputTextureSlot::Output));
				})
			->dependency({ "Lighting" })
		->node("BloomBlurPass", bloomBlurPass,
				[self = bloomBlurPass.get(), pixel = bloomPixelSelectPass.get()]() {
					self->setupInputTexture(pixel->getOutputTexture(PixelSelectRenderPass::OutputTextureSlot::SelectResult));
					self->setupBloomSize(20);
					self->setupBoommIter(2);
				})
			->dependency({ "BloomPixelSelector" })
		->node("BloomMeragePass", bloomMeragePass,
				[self = bloomMeragePass.get(), forward = mForwardRenderPass.get(), blur = bloomBlurPass]() {
					self->setupBloomTexutre(blur->getOutputTexture(BlurRenderPass::OutputTextureSlot::BlurResult));
					self->setupSrcTexutre(forward->getOutputTexture(ForwardRenderPass::OutputTextureSlot::Output));
				})
			->dependency({ "Forward","BloomBlurPass" })
		->node("Swapchain", swapChainPass,
			   [self = swapChainPass.get(), bloom = bloomMeragePass.get(), defer = mDeferRenderPass.get(), forward = mForwardRenderPass.get()]() {
					self->setupSwapChain(TheRenderSystem->window()->getSwapChain());
					self->setupTexture(bloom->getOutputTexture(BloomMerageRenderPass::OutputTextureSlot::BloomMerageResult));
					self->setupDebugTexture(forward->getOutputTexture(ForwardRenderPass::OutputTextureSlot::DebugId));
				})
			->dependency({ "BloomMeragePass","Forward"})
		->end();
}

void QRenderer::render(QRhiCommandBuffer* cmdBuffer) {
	if(mFrameGraph)
		mFrameGraph->executable(cmdBuffer);
}

void QRenderer::resize(QSize size) {
	mFrameSize = size;
	if (mFrameGraph)
		mFrameGraph->compile();
}

void QRenderer::addRenderItem(IRenderable* comp) {
	mRenderableItemList << comp;
}

void QRenderer::removeRenderItem(IRenderable* comp) {
	mRenderableItemList.removeOne(comp);
}

void QRenderer::addLightItem(ILightComponent* item) {
	mLightingRenderPass->addLightItem(item);
}

void QRenderer::removeLightItem(ILightComponent* item) {
	mLightingRenderPass->removeLightItem(item);
}

QList<IRenderable*> QRenderer::getDeferItemList() {
	QList<IRenderable*> deferList;
	for (auto& item : mRenderableItemList) {
		if (item->isDefer())
			deferList << item;
	}
	return deferList;
}

QList<IRenderable*> QRenderer::getForwardItemList() {
	QList<IRenderable*> forwardList;
	for (auto& item : mRenderableItemList) {
		if (!item->isDefer())
			forwardList << item;
	}
	return forwardList;
}

int QRenderer::getDeferPassSampleCount() {
	return 1;
}

QVector<QRhiGraphicsPipeline::TargetBlend> QRenderer::getDeferPassBlendStates() {
	return mDeferRenderPass->getBlendStates();
}

QRhiRenderPassDescriptor* QRenderer::getDeferPassDescriptor() {
	return mDeferRenderPass->getRenderTarget()->renderPassDescriptor();
}

int QRenderer::getForwardSampleCount() {
	return 1;
}

QVector<QRhiGraphicsPipeline::TargetBlend> QRenderer::getForwardPassBlendStates() {
	return mForwardRenderPass->getBlendStates();
}


QRhiRenderPassDescriptor* QRenderer::getForwardRenderPassDescriptor() {
	return mForwardRenderPass->getRenderTarget()->renderPassDescriptor();
}
