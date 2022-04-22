#include "QRenderer.h"
#include "QEngineCoreApplication.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\PixelSelectRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\BlurRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\BloomRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\SwapChainRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\LightingRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "RenderPass\DeferRenderPass.h"
#include "..\RHI\IRenderable.h"

QRenderer::QRenderer(){}

void QRenderer::buildFrameGraph() {
	QFrameGraphBuilder builder;

	mDeferRenderPass = std::make_shared<DeferRenderPass>();
	mLightingRenderPass = std::make_shared<LightingRenderPass>();

	std::shared_ptr<PixelSelectRenderPass> bloomPixelSelectPass = std::make_shared<PixelSelectRenderPass>();
	std::shared_ptr<BlurRenderPass> bloomBlurPass = std::make_shared<BlurRenderPass>();
	std::shared_ptr<BloomMerageRenderPass> bloomMeragePass = std::make_shared<BloomMerageRenderPass>();
	std::shared_ptr<SwapChainRenderPass> swapChainPass = std::make_shared<SwapChainRenderPass>();

	//	ScenePass -> LightingPass -> BloomPixelSeletorPass -> BloomBlurPass
	//	                 |									        |
	//	                 |									        V
	//					 ---------------------------------> BloomMeragePass ----> Swapchain( Scene + ImguiDebugDraw )

	mFrameGraph = builder.begin(this)			
		->node("Defer", mDeferRenderPass,[]() {})
		->node("Lighting", mLightingRenderPass,
			   [self = mLightingRenderPass.get(), scene = mDeferRenderPass.get()]() {
					self->setupBaseColorTexutre(scene->getOutputTexture(DeferRenderPass::OutputTextureSlot::BaseColor));
					self->setupPositionTexutre(scene->getOutputTexture(DeferRenderPass::OutputTextureSlot::Position));
					self->setupNormalMetalnessTexutre(scene->getOutputTexture(DeferRenderPass::OutputTextureSlot::NormalMetalness));
					self->setupTangentRoughnessTexutre(scene->getOutputTexture(DeferRenderPass::OutputTextureSlot::TangentRoughness));
				})
		->dependency({ "Defer" })
		->node("BloomPixelSelector", bloomPixelSelectPass,
			   [self = bloomPixelSelectPass.get(), lighting = mLightingRenderPass.get()]() {
					self->setDownSamplerCount(4);
					self->setupSelectCode(R"(
						void main() {
							vec4 color = texture(uTexture, vUV);
							float value = max(max(color.r,color.g),color.b);
							outFragColor = (1-step(value,1.0f)) * color;
						}
					)");
					self->setupInputTexture(lighting->getOutputTexture(LightingRenderPass::OutputTextureSlot::LightingResult));
				})
			->dependency({ "Lighting" })
		->node("BloomBlurPass", bloomBlurPass,
				[self = bloomBlurPass.get(),pixel = bloomPixelSelectPass.get()]() {
					self->setupInputTexture(pixel->getOutputTexture(PixelSelectRenderPass::OutputTextureSlot::SelectResult));
					self->setupBloomSize(20);
					self->setupBoommIter(2);
				})
			->dependency({ "BloomPixelSelector" })

		->node("BloomMeragePass", bloomMeragePass,
				[self = bloomMeragePass.get(), lighting = mLightingRenderPass.get(),blur = bloomBlurPass]() {
					self->setupBloomTexutre(blur->getOutputTexture(BlurRenderPass::OutputTextureSlot::BlurResult));
					self->setupSrcTexutre(lighting->getOutputTexture(LightingRenderPass::OutputTextureSlot::LightingResult));
				})
			->dependency({ "Defer","BloomBlurPass"})
		->node("Swapchain", swapChainPass,
			   [self = swapChainPass.get(), bloom = bloomMeragePass.get(), scene = mDeferRenderPass.get()]() {
					self->setupSwapChain(TheRenderSystem->window()->getSwapChain());
					self->setupTexture(bloom->getOutputTexture(BloomMerageRenderPass::OutputTextureSlot::BloomMerageResult));
					self->setupDebugTexture(scene->getOutputTexture(DeferRenderPass::OutputTextureSlot::DebugId));
				})
			->dependency({ "BloomMeragePass" })
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
