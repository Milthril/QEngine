#ifndef QDefaultProxySkyBox_h__
#define QDefaultProxySkyBox_h__

#include "QDefaultRenderer.h"
#include "Render\Material\QMaterialProxy.h"

class QDefaultProxySkyBox :public QRhiProxy {
public:
	QDefaultProxySkyBox(std::shared_ptr<QSkyBoxComponent> SkyBox);
private:
	std::shared_ptr<QSkyBoxComponent> mSkyBox;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiTexture> mTexture;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
protected:
	void recreateResource() override;
	void recreatePipeline(PipelineUsageFlags flags = PipelineUsageFlag::Normal) override;
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
};

#endif // QDefaultProxySkyBox_h__
