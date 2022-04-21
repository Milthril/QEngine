#ifndef QRenderer_h__
#define QRenderer_h__

#include "QFrameGraph.h"
#include "ECS/System/RenderSystem/RHI/QRhiDefine.h"
#include "ECS/System//RenderSystem/IRenderable.h"

class DeferRenderPass;

class QRenderer {
	friend class QRenderSystem;
public:
	QRenderer();
	void buildFrameGraph() ;
	void render(QRhiCommandBuffer* cmdBuffer) ;
	void resize(QSize size);

	void addRenderItem(IRenderable* comp);
	void removeRenderItem(IRenderable* comp);

	QSize getFrameSize() const { return mFrameSize; }
	const QList<IRenderable*>& getRenderableItemList() const { return mRenderableItemList; }

	int getDeferPassSampleCount();
	QVector<QRhiGraphicsPipeline::TargetBlend> getDeferPassBlendStates();
	QRhiRenderPassDescriptor* getDeferPassDescriptor();

protected:
	std::shared_ptr<QFrameGraph> mFrameGraph;
	std::shared_ptr<DeferRenderPass> mDeferRenderPass;
	QList<IRenderable*> mRenderableItemList;
	QSize mFrameSize;
};

#endif // QRenderer_h__
