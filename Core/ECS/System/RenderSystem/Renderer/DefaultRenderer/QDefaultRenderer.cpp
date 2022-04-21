#include "QDefaultRenderer.h"

#include "QEngineCoreApplication.h"
#include "QDefaultSceneRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\PixelSelectRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\BlurRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\BloomRenderPass.h"
#include "ECS\System\RenderSystem\Renderer\RenderPass\SwapChainRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"


QDefaultRenderer::QDefaultRenderer(): IRenderer(std::make_shared<QDefaultSceneRenderPass>()) {}

void QDefaultRenderer::buildFrameGraph() {
	QFrameGraphBuilder builder;

	std::shared_ptr<PixelSelectRenderPass> bloomPixelSelectPass = std::make_shared<PixelSelectRenderPass>();
	std::shared_ptr<BlurRenderPass> bloomBlurPass = std::make_shared<BlurRenderPass>();
	std::shared_ptr<BloomMerageRenderPass> bloomMeragePass = std::make_shared<BloomMerageRenderPass>();
	std::shared_ptr<SwapChainRenderPass> swapChainPass = std::make_shared<SwapChainRenderPass>();

	//	ScenePass -> BloomPixelSeletorPass -> BloomBlurPass
	//	  |											     |
	//	  |											     V
	//     ---------------------------------> BloomMeragePass ----> Swapchain( Scene + ImguiDebugDraw )

	mFrameGraph = builder.begin()			
		->node("Scene", mScenePass,
			   [self = mScenePass.get(),renderer = this]() {
					self->setupSampleCount(4);
					self->setupSceneFrameSize(TheRenderSystem->window()->getSwapChain()->currentPixelSize());
				})
		->node("BloomPixelSelector", bloomPixelSelectPass,
			   [self = bloomPixelSelectPass.get(), scene = mScenePass.get()]() {
					self->setDownSamplerCount(4);
					self->setupSelectCode(R"(
						void main() {
							vec4 color = texture(uTexture, vUV);
							float value = max(max(color.r,color.g),color.b);
							outFragColor = (1-step(value,1.0f)) * color;
						}
					)");
					self->setupInputTexture(scene->getOutputTexture());
				})
			->dependency({ "Scene" })
		->node("BloomBlurPass", bloomBlurPass,
				[self = bloomBlurPass.get(),pixel = bloomPixelSelectPass.get()]() {
					self->setupInputTexture(pixel->getOutputTexture());
					self->setupBloomSize(20);
					self->setupBoommIter(2);
				})
			->dependency({ "BloomPixelSelector" })

		->node("BloomMeragePass", bloomMeragePass,
				[self = bloomMeragePass.get(), scene = mScenePass.get(),blur = bloomBlurPass]() {
					self->setupBloomTexutre(blur->getOutputTexture());
					self->setupSrcTexutre(scene->getOutputTexture());
				})
			->dependency({ "Scene","BloomBlurPass"})
		->node("Swapchain", swapChainPass,
			   [self = swapChainPass.get(), bloom = bloomMeragePass.get(), scene = mScenePass.get()]() {
					self->setupSwapChain(TheRenderSystem->window()->getSwapChain());
					self->setupTexture(bloom->getOutputTexture());
					self->setupDebugTexture(scene->getDebugTexutre());
				})
			->dependency({ "BloomMeragePass" })
		->end();
	mFrameGraph->compile();
}

void QDefaultRenderer::render(QRhiCommandBuffer* cmdBuffer) {
	if(mFrameGraph)
		mFrameGraph->executable(cmdBuffer);
}

void QDefaultRenderer::requestReadbackCompId(const QPoint& screenPt) {

}
