#ifndef QBloomPainter_h__
#define QBloomPainter_h__

#include "QTexturePainter.h"
#include "QPixelSelector.h"

class QBloomMeragePainter {
public:
	QBloomMeragePainter();
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom, QRhiRenderTarget* renderTarget);
protected:
	void initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom);
	void updateTexture(QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom);
private:
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mSrcTexture;
	QRhiSPtr<QRhiTexture> mBloomTexture;
};

class QBloomPainter {
public:
	QBloomPainter();
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> inputTexture, QRhiRenderTarget* outputTarget);
	void setBloomSize(int size);
protected:
	void initRhiResource();
	void createOrResize(QSize size);
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
	QPixelSelector mPixelSelector;
	QBloomMeragePainter mMeragePainter;

	struct BloomState {
		uint32_t size = 0;
		uint32_t padding[3];
		float weight[40] = { 0 };
	}mBloomState;
	int mBoommIter = 2;
	QRhiSignal bNeedUpdateBloomState;
};

#endif // QBloomPainter_h__
