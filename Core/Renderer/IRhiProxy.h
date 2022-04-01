#ifndef IRhiProxy_h__
#define IRhiProxy_h__

#include "RHI/QRhiDefine.h"
#include "Scene/QSceneComponent.h"

class ISceneRenderPass;

class IRhiProxy {
public:
	virtual void recreateResource() {}
	virtual void recreatePipeline() {}
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) {}
	virtual void updateResource(QRhiResourceUpdateBatch* batch) {}
	virtual void updatePrePass(QRhiCommandBuffer* cmdBuffer) {}
	virtual void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {}
public:
	ISceneRenderPass* mRenderPass;
	std::shared_ptr<QSceneComponent> mComponent;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
};

#endif // QSceneRenderer_h__
