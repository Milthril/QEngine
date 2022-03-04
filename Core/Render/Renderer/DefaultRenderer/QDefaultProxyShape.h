#ifndef QDefaultProxyShape_h__
#define QDefaultProxyShape_h__

#include "QDefaultRenderer.h"

class QDefaultProxyShape :public QRhiProxy {
public:
	QDefaultProxyShape(std::shared_ptr<QShapeComponent> shape);
private:
	std::shared_ptr<QShapeComponent> mParticle;
	QRhiSPtr<QRhiTexture> mTexture;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
protected:
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void recreateResource() override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
};

#endif // QDefaultProxyShape_h__
