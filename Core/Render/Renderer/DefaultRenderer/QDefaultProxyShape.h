#ifndef QDefaultProxyShape_h__
#define QDefaultProxyShape_h__

#include "QDefaultRenderer.h"

class QDefaultProxyShape :public QRhiProxy {
public:
	QDefaultProxyShape(std::shared_ptr<QShapeComponent> shape);
private:
	std::shared_ptr<QShapeComponent> mShape;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiTexture> mTexture;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBinding;
protected:
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void recreateResource() override;
	void draw(QRhiCommandBuffer* cmdBuffer) override;
};

#endif // QDefaultProxyShape_h__
