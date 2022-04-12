#include "ISceneRenderPass.h"
#include "QEngine.h"

void ISceneRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {
	//QRhiResourceUpdateBatch* resUpdateBatch = RHI->nextResourceUpdateBatch();
	//QSet<QSceneComponent::ComponentId> mValidId;
	//for (int i = 0; i < primitiveList.size(); i++) {		//为新增的组件创建代理
	//	if (!mPrimitiveProxyMap.contains(primitiveList[i]->componentId())) {
	//		const auto& proxy = createPrimitiveProxy(primitiveList[i]);
	//		mPrimitiveProxyMap[primitiveList[i]->componentId()] = proxy;
	//	}
	//	mValidId.insert(primitiveList[i]->componentId());
	//}

	//for (auto& id : mPrimitiveProxyMap.keys()) {			//销毁已经失效的代理
	//	if (!mValidId.contains(id)) {
	//		mPrimitiveProxyMap.remove(id);
	//	}
	//}
	//cmdBuffer->resourceUpdate(resUpdateBatch);
	//for (auto& proxy : mPrimitiveProxyMap) {
	//	proxy->updatePrePass(cmdBuffer);
	//}
	//resUpdateBatch = RHI->nextResourceUpdateBatch();
	//for (auto& proxy : mPrimitiveProxyMap) {
	//	if (proxy->mComponent->bNeedRecreateResource.receive()) {
	//		proxy->recreateResource();
	//		proxy->uploadResource(resUpdateBatch);
	//	}
	//	proxy->updateResource(resUpdateBatch);
	//	if (proxy->mComponent->bNeedRecreatePipeline.receive()) {
	//		proxy->recreatePipeline();
	//	}
	//}
	//cmdBuffer->beginPass(getRenderTarget(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f), { 1.0f, 0 }, resUpdateBatch);
	//QRhiViewport viewport(0, 0, getRenderTarget()->pixelSize().width(), getRenderTarget()->pixelSize().height());
	//for (auto& proxy : mPrimitiveProxyMap) {
	//	if (proxy->mComponent->bNeedRecreatePipeline.receive()) {
	//		proxy->recreatePipeline();
	//	}
	//	proxy->drawInPass(cmdBuffer, viewport);
	//}
	//cmdBuffer->endPass();
}

void ISceneRenderPass::setupSceneFrameSize(QSize size) {
	mSceneFrameSize = size;
}

void ISceneRenderPass::setupSampleCount(int count) {
	mSampleCount = count;
}
