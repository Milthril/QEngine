#ifndef QStaticMeshComponent_h__
#define QStaticMeshComponent_h__

#include "qvectornd.h"
#include "IRenderableComponent.h"
#include "Asset\Material.h"
#include "Asset\StaticMesh.h"

class QStaticMeshComponent :public IRenderableComponent {
	Q_OBJECT
		Q_COMPONENT(QStaticMeshComponent)
		Q_PROPERTY(std::shared_ptr<Asset::StaticMesh> StaticMesh READ getStaticMesh WRITE setStaticMesh)
		Q_PROPERTY(std::shared_ptr<Asset::Material> Material READ getMaterial WRITE setMaterial)
public:
	const std::shared_ptr<Asset::StaticMesh>& getStaticMesh() const { return mStaticMesh; }
	void setStaticMesh(std::shared_ptr<Asset::StaticMesh> val);

	std::shared_ptr<Asset::Material> getMaterial() { 
		return mMaterial;
	}
	void setMaterial(std::shared_ptr<Asset::Material> val);

	virtual void recreateResource() override;
	virtual void recreatePipeline() override;
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) override;
	virtual void updatePrePass(QRhiCommandBuffer* cmdBuffer) override;
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) override;
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;

private:
	std::shared_ptr<Asset::StaticMesh> mStaticMesh;
	std::shared_ptr<Asset::Material> mMaterial;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
};

#endif // QStaticMeshComponent_h__