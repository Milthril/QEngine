#ifndef QSkyBoxComponent_h__
#define QSkyBoxComponent_h__

#include "IRenderableComponent.h"
#include "Asset\SkyBox.h"

class QSkyBoxComponent :public IRenderableComponent {
	Q_OBJECT
		Q_PROPERTY(std::shared_ptr<Asset::SkyBox> SkyBox READ getSkyBox WRITE setSkyBox)
public:

	const std::shared_ptr<Asset::SkyBox>& getSkyBox() const { return mSkyBox; }
	void setSkyBox(std::shared_ptr<Asset::SkyBox> val);

	virtual void recreateResource() override;
	virtual void recreatePipeline() override;
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) override;
	virtual void updatePrePass(QRhiCommandBuffer* cmdBuffer) override;
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) override;
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;

private:
	std::shared_ptr<Asset::SkyBox> mSkyBox;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
	QRhiSPtr<QRhiTexture> mTexture;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
};

#endif // QSkyBoxComponent_h__