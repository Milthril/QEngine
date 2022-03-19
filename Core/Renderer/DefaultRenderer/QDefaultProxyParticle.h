#ifndef QDefaultProxyParticle_h__
#define QDefaultProxyParticle_h__

#include "QDefaultRenderer.h"
#include "Scene\Component\Particle\QParticleComponent.h"


class QDefaultProxyStaticMesh;

class QDefaultProxyParticle :public QRhiProxy {
public:
	QDefaultProxyParticle(std::shared_ptr<QParticleComponent> shape);
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
private:
	std::shared_ptr<QParticleComponent> mParticle;

	struct ParticleRunContext {
		uint inputCounter = 0;
		uint outputCounter = 0;
		float duration = 0;
		float lifetime = 0;
	};

	QRhiSPtr<QRhiBuffer> mParticlesBuffer[2];
	QRhiSPtr<QRhiBuffer> mParticleCounterBuffer;
	QRhiSPtr<QRhiComputePipeline> mComputePipeline;
	QRhiSPtr<QRhiShaderResourceBindings> mComputeBindings[2];
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;

	QRhiSPtr<QRhiBuffer> mMatrixBuffer;
	QRhiSPtr<QRhiComputePipeline> mMatrixComputePipline;
	QRhiSPtr<QRhiShaderResourceBindings> mMatrixBindings[2];

	QRhiBufferReadbackResult mCtxReader;
	ParticleRunContext mCtx;
	int mInputIndex = 0;
	int mOutputIndex = 1;
	float mDuration;
	float mLastSecond;
protected:
	void recreateResource() override;
	void recreatePipeline() override;
	void updatePrePass(QRhiCommandBuffer* cmdBuffer) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
	std::shared_ptr<QDefaultProxyStaticMesh> mStaticMeshProxy;
};

#endif // QDefaultProxyParticle_h__
