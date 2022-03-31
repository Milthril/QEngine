#ifndef ISceneRenderPass_h__
#define ISceneRenderPass_h__

#include "IRenderPassBase.h"

class QSceneRenderPass :public QRenderPass {
public:
	QSceneRenderPass() {}
	virtual int getSampleCount() = 0;
	virtual QRhiRenderPassDescriptor* getRenderPassDescriptor() = 0;
	virtual QVector<QRhiGraphicsPipeline::TargetBlend>  getBlendStates() = 0;

	bool getEnableOutputDebugId() const { return mEnableOutputDebugId; }
	void setEnableOutputDebugId(bool val) { mEnableOutputDebugId = val; }
protected:
	bool mEnableOutputDebugId = false;
};

#endif // ISceneRenderPass_h__
