#ifndef QDefaultProxyParticle_h__
#define QDefaultProxyParticle_h__

#include "QDefaultRenderer.h"

class QDefaultProxyParticle :public QRhiProxy {
public:
	QDefaultProxyParticle(std::shared_ptr<QParticleComponent> shape);
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
private:
	std::shared_ptr<QParticleComponent> mParticle;

	QRhiSPtr<QRhiBuffer> mParticlesBuffer[2];
	QRhiSPtr<QRhiComputePipeline> mComputePipeline;
	QRhiSPtr<QRhiShaderResourceBindings> mComputeBindings[2];
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;

	QRhiSPtr<QRhiBuffer> mMatrixBuffer;
	QRhiSPtr<QRhiComputePipeline> mMatrixComputePipline;
	QRhiSPtr<QRhiShaderResourceBindings> mMatrixBindings[2];
	QRhiBufferReadbackResult mNumReader;
	int mNumOfParticles = 0;
	int mInputIndex = 0;
	int mOutputIndex = 1;

protected:
	void recreateResource() override;
	void recreateGraphicsPipline();
	void recreateComputePipeline();
	void updatePrePass(QRhiCommandBuffer* cmdBuffer) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
	std::shared_ptr<QRhiProxy> mShapeProxy;
};

#endif // QDefaultProxyParticle_h__
