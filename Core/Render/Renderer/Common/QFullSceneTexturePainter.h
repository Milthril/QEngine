#ifndef QFullSceneTexturePainter_h__
#define QFullSceneTexturePainter_h__

#include "Render\Renderer\QSceneRenderer.h"

class QFullSceneTexturePainter {
public:
	QFullSceneTexturePainter(std::shared_ptr<QRhi> rhi);
	void initRhiResource(QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc, int sampleCount);
	void updateTexture(QRhiSPtr<QRhiTexture> texture);
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget);
private:
	std::shared_ptr<QRhi> mRhi;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mTexture;
};

#endif // QFullSceneTexturePainter_h__
