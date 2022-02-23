#ifndef QDefaultProxyText2D_h__
#define QDefaultProxyText2D_h__

#include "QDefaultRenderer.h"
#include "QFont"
#include "QBrush"

class QDefaultProxyText2D :public QRhiProxy {
public:
	QDefaultProxyText2D(std::shared_ptr<QText2DComponent> text);
	static QImage createTextImage(const QString& text, const QFont& font, const QBrush& brush, const int& spacing = 0, Qt::Orientation o = Qt::Orientation::Horizontal);
	void recreateResource() override;
private:
	std::shared_ptr<QText2DComponent> mText2D;
	std::shared_ptr<QRhiBuffer> mVertexBuffer;
	std::shared_ptr<QRhiTexture> mTexture;
	std::shared_ptr<QRhiSampler> mSampler;
	std::shared_ptr<QRhiBuffer> mUniformBuffer;
	std::shared_ptr<QRhiShaderResourceBindings> mShaderResourceBinding;
	bool mInit = false;
	QImage mTextImage;

protected:
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void draw(QRhiCommandBuffer* cmdBuffer) override;
};

#endif // QDefaultProxyText2D_h__
