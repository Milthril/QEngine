#ifndef QRenderer_h__
#define QRenderer_h__

#include "QFrameGraph.h"
#include "ECS/System/RenderSystem/RHI/QRhiDefine.h"

class DeferRenderPass;
class ILightComponent;
class IRenderable;
class LightingRenderPass;
class ForwardRenderPass;

class QRenderer {
	friend class QRenderSystem;
public:
	QRenderer();
	void buildFrameGraph() ;
	void render(QRhiCommandBuffer* cmdBuffer) ;
	void resize(QSize size);

	void addRenderItem(IRenderable* comp);
	void removeRenderItem(IRenderable* comp);

	void addLightItem(ILightComponent* item);
	void removeLightItem(ILightComponent* item);

	QSize getFrameSize() const { return mFrameSize; }

	QList<IRenderable*> getDeferItemList();
	QList<IRenderable*> getForwardItemList();

	int getDeferPassSampleCount();
	QVector<QRhiGraphicsPipeline::TargetBlend> getDeferPassBlendStates();
	QRhiRenderPassDescriptor* getDeferPassDescriptor();

	int getForwardSampleCount();
	QVector<QRhiGraphicsPipeline::TargetBlend> getForwardPassBlendStates();
	QRhiRenderPassDescriptor* getForwardRenderPassDescriptor();

	std::shared_ptr<QFrameGraph> getFrameGraph() const { return mFrameGraph; }
protected:
	QList<IRenderable*> mRenderableItemList;
	std::shared_ptr<QFrameGraph> mFrameGraph;

	std::shared_ptr<DeferRenderPass> mDeferRenderPass;
	std::shared_ptr<ForwardRenderPass> mForwardRenderPass;
	std::shared_ptr<LightingRenderPass> mLightingRenderPass;
	QSize mFrameSize;
};

#endif // QRenderer_h__
