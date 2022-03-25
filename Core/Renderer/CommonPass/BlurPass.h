#ifndef BlurPass_h__
#define BlurPass_h__

#include "RHI\QRhiSignal.h"
#include "RHI\QRhiDefine.h"

class BlurPass {
public:
	BlurPass();
	void makeBlur(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> inputTexture);
	void setBloomSize(int size);
	void createOrResize(QSize size);
	QRhiSPtr<QRhiTextureRenderTarget> getInputRenderTaget() { return mBloomRT[0].renderTarget; }
	QRhiSPtr<QRhiTexture> getOutputTexture() { return mBloomRT[0].colorAttachment; }
protected:
	void initRhiResource();
private:
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	struct BloomRT {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
	};
	BloomRT mBloomRT[2];

	QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	QRhiSPtr<QRhiGraphicsPipeline> mPipelineH;
	QRhiSPtr<QRhiGraphicsPipeline> mPipelineV;
	QRhiSPtr<QRhiShaderResourceBindings> mBindingsH;
	QRhiSPtr<QRhiShaderResourceBindings> mBindingsV;
	struct BloomState {
		uint32_t size = 0;
		uint32_t padding[3];
		float weight[40] = { 0 };
	}mBloomState;
	int mBoommIter = 2;
	QRhiSignal bNeedUpdateBloomState;
};

#endif // BlurPass_h__
