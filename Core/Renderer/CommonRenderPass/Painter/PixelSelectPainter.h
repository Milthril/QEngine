#ifndef PixelSelectPainter_h__
#define PixelSelectPainter_h__

#include "Renderer/IRenderPassBase.h"

class PixelSelectPainter :public IRenderPassBase{
public:
	PixelSelectPainter();
	void setupSelectCode(QByteArray code);
	void setupInputTexture(QRhiTexture* texture);
	void setDownSamplerCount(int count);
	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	QRhiTexture* getOutputTexture() { return mRT.colorAttachment.get(); }
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	};
	RTResource mRT;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiTexture* mInputTexture;
	QByteArray mSelectCode;
	uint32_t mDownSamplerCount = 1;
};

#endif // PixelSelectPainter_h__
