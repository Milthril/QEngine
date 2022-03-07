#ifndef QDefaultProxyParticle_h__
#define QDefaultProxyParticle_h__

#include "QDefaultRenderer.h"
#include "Render\Scene\Component\QParticleComponent.h"

class QDefaultProxyParticle :public QRhiProxy {
public:
	QDefaultProxyParticle(std::shared_ptr<QParticleComponent> shape);
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
private:
	uint32_t allocIndex();
private:
	std::shared_ptr<QParticleComponent> mParticle;

	QRhiSPtr<QRhiBuffer> mParticlesBuffer[2];
	QRhiSPtr<QRhiComputePipeline> mComputePipeline;
	QRhiSPtr<QRhiShaderResourceBindings> mComputeBindings[2];
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;

	QRhiSPtr<QRhiBuffer> mMatrixBuffer;
	QRhiSPtr<QRhiComputePipeline> mMatrixComputePipline;
	QRhiSPtr<QRhiShaderResourceBindings> mMatrixBindings[2];

	int mInputIndex = 0;
	int mOutputIndex = 1;

	std::array<float, QParticleComponent::PARTICLE_MAX_SIZE> mAgePool = { 0 };
	QList<uint32_t> mIndexPool;
	int mNumOfParticles = 0;
	float mDuration;
	float mLastSecond;
protected:
	void recreateResource() override;
	void recreatePipeline(PipelineUsageFlags flags = PipelineUsageFlag::Normal) override;
	void updatePrePass(QRhiCommandBuffer* cmdBuffer) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
	std::shared_ptr<QRhiProxy> mShapeProxy;
};

#endif // QDefaultProxyParticle_h__
