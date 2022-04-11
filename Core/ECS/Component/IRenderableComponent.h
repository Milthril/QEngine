#ifndef IRenderableComponent_h__
#define IRenderableComponent_h__

#include "RHI\QRhiDefine.h"
#include "IComponent.h"

class IRenderableComponent : public IComponent {
public:
	virtual ~IRenderableComponent();
	virtual void recreateResource() {}
	virtual void recreatePipeline() {}
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) {}
	virtual void updatePrePass(QRhiCommandBuffer* cmdBuffer) {}
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) {}
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer) = 0;

	QRhiSignal bNeedRecreateResource;
	QRhiSignal bNeedRecreatePipeline;

	virtual void setupEntity(QEntity* entity) override;
};

#endif // IRenderableComponent_h__
