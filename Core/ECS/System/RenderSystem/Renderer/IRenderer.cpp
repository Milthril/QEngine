#include "IRenderer.h"
#include "RenderPass/ISceneRenderPass.h"

IRenderer::IRenderer(std::shared_ptr<ISceneRenderPass> scenePass) 
	:mScenePass(scenePass) {
}
