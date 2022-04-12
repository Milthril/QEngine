#include "ISceneRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "ECS\System\RenderSystem\IRenderable.h"

void ISceneRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {
	for (auto& item : mRenderItemList) {
		item->updatePrePass(cmdBuffer);
	}
	QRhiResourceUpdateBatch* resUpdateBatch = RHI->nextResourceUpdateBatch();
	for (auto& item : mRenderItemList) {
		if (item->bNeedRecreateResource.receive()) {
			item->recreateResource();
			item->uploadResource(resUpdateBatch);
		}
		item->updateResourcePrePass(resUpdateBatch);
		if (item->bNeedRecreatePipeline.receive()) {
			item->recreatePipeline();
		}
	}
	cmdBuffer->beginPass(getRenderTarget(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f), { 1.0f, 0 }, resUpdateBatch);
	QRhiViewport viewport(0, 0, getRenderTarget()->pixelSize().width(), getRenderTarget()->pixelSize().height());
	//cmdBuffer->setViewport(viewport);
	for (auto& item : mRenderItemList) {
		if (item->bNeedRecreatePipeline.receive()) {
			item->recreatePipeline();
		}
		item->renderInPass(cmdBuffer);
	}
	cmdBuffer->endPass();
}

void ISceneRenderPass::setupSceneFrameSize(QSize size) {
	mSceneFrameSize = size;
}

void ISceneRenderPass::setupSampleCount(int count) {
	mSampleCount = count;
}
