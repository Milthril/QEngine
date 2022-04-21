#ifndef BlurRenderPass_h__
#define BlurRenderPass_h__

#include "IRenderPassBase.h"

class BlurRenderPass: public IRenderPassBase {
public:
	BlurRenderPass();

	void setupInputTexture(QRhiTexture* inputTexture);
	void setupBloomSize(int size);
	void setupBoommIter(int val) { mBoommIter = val; }

	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	int getBoommIter() const { return mBoommIter; }
	QRhiTextureRenderTarget* getInputRenderTaget() { return mBloomRT[0].renderTarget.get(); }

	enum OutputTextureSlot {
		BlurResult = 0,
	};
	virtual QRhiTexture* getOutputTexture(int slot = 0) {
		switch ((OutputTextureSlot)slot) {
		case BlurResult:
			return mBloomRT[0].colorAttachment.get();	
		}
		return nullptr;
	}
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

#endif // BlurRenderPass_h__
