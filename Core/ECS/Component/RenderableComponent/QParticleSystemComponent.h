#ifndef QParticleSystemComponent_h__
#define QParticleSystemComponent_h__

#include "qvectornd.h"
#include "IRenderableComponent.h"
#include "Asset\Material.h"
#include "Asset\StaticMesh.h"
#include "Asset\PartcleSystem\ParticleSystem.h"
#include "qmetatype.h"

class QParticleSystemComponent :public IRenderableComponent {
	Q_OBJECT
		Q_COMPONENT(QParticleSystemComponent)
		Q_PROPERTY(std::shared_ptr<Asset::ParticleSystem> ParticleSystem READ getParticleSystem WRITE setParticleSystem)
		Q_PROPERTY(std::shared_ptr<Asset::StaticMesh> StaticMesh READ getStaticMesh WRITE setStaticMesh)
		Q_PROPERTY(std::shared_ptr<Asset::Material> Material READ getMaterial WRITE setMaterial)
public:
	const std::shared_ptr<Asset::StaticMesh>& getStaticMesh() const;
	void setStaticMesh(std::shared_ptr<Asset::StaticMesh> val);

	std::shared_ptr<Asset::Material> getMaterial();
	void setMaterial(std::shared_ptr<Asset::Material> val);

	std::shared_ptr<Asset::ParticleSystem> getParticleSystem() const;
	void setParticleSystem(std::shared_ptr<Asset::ParticleSystem> val);

	virtual void recreateResource() override;
	virtual void recreatePipeline() override;
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) override;
	virtual void updatePrePass(QRhiCommandBuffer* cmdBuffer) override;
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) override;
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
	virtual bool isDefer() override;
private:
	std::shared_ptr<Asset::StaticMesh> mStaticMesh;
	std::shared_ptr<Asset::Material> mMaterial;
	std::shared_ptr<Asset::ParticleSystem> mParticleSystem = std::make_shared<Asset::ParticleSystem>();

	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;

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

	QRhiSPtr<QRhiBuffer> mMatrixBuffer;
	QRhiSPtr<QRhiComputePipeline> mMatrixComputePipline;
	QRhiSPtr<QRhiShaderResourceBindings> mMatrixBindings[2];

	QRhiBufferReadbackResult mCtxReader;
	ParticleRunContext mCtx;
	int mInputIndex = 0;
	int mOutputIndex = 1;
	float mDuration;
	float mLastSecond;

};

#endif // QParticleSystemComponent_h__