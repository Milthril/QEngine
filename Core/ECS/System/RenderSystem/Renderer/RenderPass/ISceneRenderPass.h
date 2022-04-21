#ifndef ISceneRenderPass_h__
#define ISceneRenderPass_h__

#include "IRenderPassBase.h"

class IRenderable;

class ISceneRenderPass :public IRenderPassBase {
	friend class QRenderSystem;
public:
	ISceneRenderPass() {}
	
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override final;
	virtual  QRhiTexture* getDebugTexutre() = 0;
	virtual QVector<QRhiGraphicsPipeline::TargetBlend>  getBlendStates() = 0;
	virtual QRhiRenderTarget* getRenderTarget() = 0;

	void setupSceneFrameSize(QSize size);
	void setupSampleCount(int count);

	virtual int getSampleCount() {
		return mSampleCount;
	}
	QRhiRenderPassDescriptor* getRenderPassDescriptor() {
		return getRenderTarget()->renderPassDescriptor();
	}
protected:
	QList<IRenderable*> mRenderItemList;
	QSize mSceneFrameSize;
	int mSampleCount = 1;
};

#endif // ISceneRenderPass_h__
