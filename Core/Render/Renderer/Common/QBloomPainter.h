#ifndef QBloomPainter_h__
#define QBloomPainter_h__

#include "Render\Renderer\QSceneRenderer.h"

class QBloomPainter {
public:
	QBloomPainter(QSceneRenderer* renderer);
	void drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget);
protected:

	void initRhiResource();

	void createOrResize(QSize size);
private:
	QSceneRenderer* mRenderer;
	QRhiSPtr<QRhiSampler> mSampler;
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
};

#endif // QBloomPainter_h__
