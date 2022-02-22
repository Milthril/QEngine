#ifndef QDefaultProxyShape_h__
#define QDefaultProxyShape_h__

#include "QDefaultRenderer.h"

class QDefaultProxyShape :public QRhiProxy {
public:
	QDefaultProxyShape(QDefaultRenderer* renderer, std::shared_ptr<QShapeComponent> shape);
private:
	std::shared_ptr<QShapeComponent> mShape;
	std::shared_ptr<QRhiBuffer> mVertexBuffer;
	std::shared_ptr<QRhiBuffer> mIndexBuffer;
	std::shared_ptr<QRhiBuffer> mUniformBuffer;
	std::shared_ptr<QRhiShaderResourceBindings> mShaderResourceBinding;
	bool m_vbufReady = false;

protected:
	void updateResource(QRhiResourceUpdateBatch* batch, QMatrix4x4 VP) override;

	void draw(QRhiCommandBuffer* cmdBuffer) override;
};

#endif // QDefaultProxyShape_h__
