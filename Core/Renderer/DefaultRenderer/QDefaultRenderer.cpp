#include "QDefaultRenderer.h"

#include "QEngine.h"
#include "QDefaultSceneRenderPass.h"
#include "..\CommonRenderPass\SwapChainRenderPass.h"

QDefaultRenderer::QDefaultRenderer() {

}

void QDefaultRenderer::buildFrameGraph() {
	QFrameGraphBuilder builder;
	std::shared_ptr<QDefaultSceneRenderPass> scenePass = std::make_shared<QDefaultSceneRenderPass>(mScene);
	std::shared_ptr<SwapChainRenderPass> swapChainPass = std::make_shared<SwapChainRenderPass>();

	mFrameGraph = builder.begin()
		->node("Scene", scenePass,
			   [self = scenePass.get()]() {
		self->setupSampleCount(4);
		self->setupSceneFrameSize(Engine->window()->getSwapChain()->currentPixelSize());
	})
		->node("Swapchain", swapChainPass,
			   [self = swapChainPass.get(), scene = scenePass.get()]() {
		self->setupSwapChain(Engine->window()->getSwapChain());
		self->setupTexture(scene->getOutputTexture());
	})
		->dependency({ "Scene" })
		->end();
	mFrameGraph->compile();
}


void QDefaultRenderer::render() {
	mFrameGraph->executable();
}

void QDefaultRenderer::requestReadbackCompId(const QPoint& screenPt) {
	mReadPoint = screenPt;
}
