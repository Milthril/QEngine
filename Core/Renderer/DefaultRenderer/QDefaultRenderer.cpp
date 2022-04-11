#include "QDefaultRenderer.h"

#include "QEngine.h"
#include "QDefaultSceneRenderPass.h"
#include "Renderer\CommonRenderPass\SwapChainRenderPass.h"
#include "Renderer\CommonRenderPass\BloomRenderPass.h"

QDefaultRenderer::QDefaultRenderer() {}

void QDefaultRenderer::buildFrameGraph() {
	QFrameGraphBuilder builder;
	std::shared_ptr<QDefaultSceneRenderPass> scenePass = std::make_shared<QDefaultSceneRenderPass>(mScene);
	std::shared_ptr<PixelSelectPainter> bloomPixelSelectPass = std::make_shared<PixelSelectPainter>();
	std::shared_ptr<BlurPainter> bloomBlurPass = std::make_shared<BlurPainter>();
	std::shared_ptr<BloomMerageRenderPass> bloomMeragePass = std::make_shared<BloomMerageRenderPass>();
	std::shared_ptr<SwapChainRenderPass> swapChainPass = std::make_shared<SwapChainRenderPass>();

	//	ScenePass -> BloomPixelSeletorPass -> BloomBlurPass
	//	  |											     |
	//	  |											     V
	//     ---------------------------------> BloomMeragePass ----> Swapchain( Scene + ImguiDebugDraw )

	mFrameGraph = builder.begin()			
		->node("Scene", scenePass,
			   [self = scenePass.get(),renderer = this]() {
					self->setupSampleCount(4);
					self->setupSceneFrameSize(Engine->window()->getSwapChain()->currentPixelSize());
					if (renderer->getEnableDebug()) {
						self->setEnableOutputDebugId(true);
					}
				})
		->node("BloomPixelSelector", bloomPixelSelectPass,
			   [self = bloomPixelSelectPass.get(), scene = scenePass.get()]() {
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
				[self = bloomMeragePass.get(), scene = scenePass.get(),blur = bloomBlurPass]() {
					self->setupBloomTexutre(blur->getOutputTexture());
					self->setupSrcTexutre(scene->getOutputTexture());
				})
			->dependency({ "Scene","BloomBlurPass"})
		->node("Swapchain", swapChainPass,
			   [self = swapChainPass.get(), bloom = bloomMeragePass.get(), renderer = this, scene = scenePass.get()]() {
					self->setupSwapChain(Engine->window()->getSwapChain());
					self->setupTexture(bloom->getOutputTexture());
					if (renderer->getEnableDebug()) {
						self->setupDebugTexture(scene->getDebugTexutre());
					}
				})
			->dependency({ "BloomMeragePass" })
		->end();
	mFrameGraph->compile();
}

void QDefaultRenderer::render(QRhiCommandBuffer* cmdBuffer) {
	mFrameGraph->executable(cmdBuffer);

}

void QDefaultRenderer::requestReadbackCompId(const QPoint& screenPt) {

}
