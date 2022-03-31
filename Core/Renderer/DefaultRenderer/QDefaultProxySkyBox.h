#ifndef QDefaultProxySkyBox_h__
#define QDefaultProxySkyBox_h__

#include "Renderer\ISceneComponentRenderProxy.h"
#include "Scene\Component\SkyBox\QSkyBoxComponent.h"

class QDefaultProxySkyBox :public ISceneComponentRenderProxy {
public:
	QDefaultProxySkyBox(std::shared_ptr<QSkyBoxComponent> SkyBox);
private:
	std::shared_ptr<QSkyBoxComponent> mSkyBox;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiTexture> mTexture;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
protected:
	void recreateResource() override;
	void recreatePipeline() override;
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
};

#endif // QDefaultProxySkyBox_h__
