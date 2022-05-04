#ifndef BlurRenderPass_h__
#define BlurRenderPass_h__

#include "IRenderPassBase.h"

class BlurRenderPass: public IRenderPassBase {
	Q_OBJECT
		Q_PROPERTY(int BlurIterations READ getBlurIter WRITE setupBlurIter)
		Q_PROPERTY(int BlurSize READ getBlurSize WRITE setupBlurSize)
public:
	BlurRenderPass();

	void setupBlurSize(int size);
	void setupBlurIter(int val);

	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	int getBlurIter() const { return mBlurIter; }
	int getBlurSize() const { return mBlurState.size; }
	QRhiTextureRenderTarget* getInputRenderTaget() { return mBlurRT[0].renderTarget.get(); }

	enum InputTextureSlot {
		Color = 0,
	};

	enum OutputTextureSlot {
		BlurResult = 0,
	};
private:
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	struct BlurRT {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
	};
	BlurRT mBlurRT[2];
	QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	QRhiSPtr<QRhiGraphicsPipeline> mPipelineH;
	QRhiSPtr<QRhiGraphicsPipeline> mPipelineV;
	QRhiSPtr<QRhiShaderResourceBindings> mBindingsH;
	QRhiSPtr<QRhiShaderResourceBindings> mBindingsV;
	struct BlurState {
		uint32_t size = 0;
		uint32_t padding[3];
		float weight[40] = { 0 };
	}mBlurState;
	int mBlurIter = 2;
	QRhiSignal bNeedUpdateBlurState;
};

#endif // BlurRenderPass_h__
