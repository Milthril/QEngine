#ifndef QDefaultProxyShape_h__
#define QDefaultProxyShape_h__

#include "QDefaultRenderer.h"

class QDefaultProxyShape :public QRhiProxy {
public:
	QDefaultProxyShape(std::shared_ptr<QShapeComponent> shape);

private:
	std::shared_ptr<QShapeComponent> mShape;
	std::shared_ptr<QRhiBuffer> mVertexBuffer;
	std::shared_ptr<QRhiBuffer> mIndexBuffer;
	std::shared_ptr<QRhiBuffer> mUniformBuffer;
	std::shared_ptr<QRhiTexture> mTexture;
	std::shared_ptr<QRhiSampler> mSampler;
	std::shared_ptr<QRhiShaderResourceBindings> mShaderResourceBinding;
protected:
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void recreateResource() override;
	void draw(QRhiCommandBuffer* cmdBuffer) override;
};

#endif // QDefaultProxyShape_h__
