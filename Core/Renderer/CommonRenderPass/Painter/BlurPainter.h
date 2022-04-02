#ifndef BlurPainter_h__
#define BlurPainter_h__

#include "Renderer/IRenderPassBase.h"

class BlurPainter: public IRenderPassBase {
public:
	BlurPainter();

	void setupInputTexture(QRhiTexture* inputTexture);

	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	void setBloomSize(int size);
	int getBoommIter() const { return mBoommIter; }
	void setBoommIter(int val) { mBoommIter = val; }

	QRhiTextureRenderTarget* getInputRenderTaget() { return mBloomRT[0].renderTarget.get(); }
	QRhiTexture* getOutputTexture() { return mBloomRT[0].colorAttachment.get(); }
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
	QRhiTexture* mInputTexture;
	struct BloomState {
		uint32_t size = 0;
		uint32_t padding[3];
		float weight[40] = { 0 };
	}mBloomState;
	int mBoommIter = 2;

	QRhiSignal bNeedUpdateBloomState;
};

#endif // BlurPainter_h__
