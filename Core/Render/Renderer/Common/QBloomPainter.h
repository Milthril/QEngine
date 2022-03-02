#ifndef QBloomPainter_h__
#define QBloomPainter_h__

#include "Render\Renderer\QSceneRenderer.h"
#include "QFullSceneTexturePainter.h"
#include "QPixelSelector.h"

class QBloomMeragePainter {
public:
	QBloomMeragePainter(std::shared_ptr<QRhi> rhi);
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom, QRhiRenderTarget* renderTarget);
protected:
	void initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom);
	void updateTexture(QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom);
private:
	std::shared_ptr<QRhi> mRhi;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mSrcTexture;
	QRhiSPtr<QRhiTexture> mBloomTexture;
};

class QBloomPainter {
public:
	QBloomPainter(std::shared_ptr<QRhi> rhi);
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> inputTexture, QRhiRenderTarget* outputTarget);
	void setBloomSize(int size);
protected:
	void initRhiResource();
	void createOrResize(QSize size);
private:
	std::shared_ptr<QRhi> mRhi;
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
	QFullSceneTexturePainter mWriteBackPainter;
	QPixelSelector mPixelSelector;
	QBloomMeragePainter mMeragePainter;

	struct BloomState {
		uint32_t size = 0;
		uint32_t padding[3];
		float weight[40] = { 0 };
	}mBloomState;
	int mBoommIter = 2;
	uint8_t bNeedUpdateBloomState : 1 = false;
};

#endif // QBloomPainter_h__
