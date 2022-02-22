#ifndef QDefaultProxyText2D_h__
#define QDefaultProxyText2D_h__

#include "QDefaultRenderer.h"

class QDefaultProxyText2D :public QRhiProxy {
public:
	QDefaultProxyText2D(QDefaultRenderer* renderer, std::shared_ptr<QText2DComponent> shape);
private:
	std::shared_ptr<QText2DComponent> mText2D;
	std::shared_ptr<QRhiBuffer> mVertexBuffer;
	std::shared_ptr<QRhiBuffer> mUniformBuffer;
	std::shared_ptr<QRhiShaderResourceBindings> mShaderResourceBinding;
	bool m_vbufReady = false;
protected:
	void updateResource(QRhiResourceUpdateBatch* batch, QMatrix4x4 VP) override;

	void draw(QRhiCommandBuffer* cmdBuffer) override;
};

#endif // QDefaultProxyText2D_h__
