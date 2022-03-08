#ifndef QDefaultProxyShape_h__
#define QDefaultProxyShape_h__

#include "QDefaultRenderer.h"
#include "Render\Material\QMaterialProxy.h"

class QDefaultProxyShape :public QRhiProxy {
public:
	QDefaultProxyShape(std::shared_ptr<QShapeComponent> shape);
private:
	std::shared_ptr<QShapeComponent> mShape;
	QRhiSPtr<QRhiTexture> mTexture;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
	std::shared_ptr<QMaterialProxy> mMaterialProxy;
protected:
	void recreateResource() override;
	void recreatePipeline(PipelineUsageFlags flags = PipelineUsageFlag::Normal) override;
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
};

#endif // QDefaultProxyShape_h__
