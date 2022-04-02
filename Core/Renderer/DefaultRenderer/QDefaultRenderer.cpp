#include "QDefaultRenderer.h"

#include "QEngine.h"
#include "QDefaultSceneRenderPass.h"
#include "Renderer\CommonRenderPass\SwapChainRenderPass.h"
#include "Renderer\CommonRenderPass\BloomRenderPass.h"
#include "Renderer\CommonRenderPass\DebugRenderPasss.h"

QDefaultRenderer::QDefaultRenderer() {}

void QDefaultRenderer::buildFrameGraph() {
	QFrameGraphBuilder builder;
	std::shared_ptr<QDefaultSceneRenderPass> scenePass = std::make_shared<QDefaultSceneRenderPass>(mScene);
	std::shared_ptr<BloomRenderPass> bloomPass = std::make_shared<BloomRenderPass>();
	std::shared_ptr<SwapChainRenderPass> swapChainPass = std::make_shared<SwapChainRenderPass>();
	std::shared_ptr<DebugRenderPasss> debugPass = std::make_shared<DebugRenderPasss>();

	mFrameGraph = builder.begin()
		->node("Scene", scenePass,
			   [self = scenePass.get()]() {
					self->setupSampleCount(4);
					self->setupSceneFrameSize(Engine->window()->getSwapChain()->currentPixelSize());
				})
		//->node("Bloom",bloomPass,
		//	   [self = bloomPass.get(), scene = scenePass.get()]() {
		//			self->setupInputTexture(scene->getOutputTexture());
		//	   })
		//	->dependency({ "Scene" })
		->node("Swapchain", swapChainPass,
			   [self = swapChainPass.get(), scene = scenePass.get()]() {
					self->setupSwapChain(Engine->window()->getSwapChain());
					self->setupTexture(scene->getOutputTexture());
				})
			->dependency({ "Scene" })
		//->node("Debug", debugPass,
		//				   [self = debugPass.get()]() {
		//			self->setupWindow(Engine->window().get());
		//		})
		//	->dependency({ "Swapchain" })
		->end();

	mFrameGraph->compile();
}

void QDefaultRenderer::render() {
	mFrameGraph->executable();
}

void QDefaultRenderer::requestReadbackCompId(const QPoint& screenPt) {
	mReadPoint = screenPt;
}
