#ifndef IRenderable_h__
#define IRenderable_h__

#include "QRhiDefine.h"

class IRenderable {
public:
	virtual void recreateResource() {}
	virtual void recreatePipeline() {}
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) {}
	virtual void updatePrePass(QRhiCommandBuffer* cmdBuffer) {}
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) {}
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) = 0;
	virtual bool isDefer() { return false; }
	QRhiSignal bNeedRecreateResource;
	QRhiSignal bNeedRecreatePipeline;

	void active();
	void deactive();
};

#endif // IRenderable_h__
