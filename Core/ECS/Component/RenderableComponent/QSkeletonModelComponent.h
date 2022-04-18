#ifndef QSkeletonModelComponent_h__
#define QSkeletonModelComponent_h__

#include "IRenderableComponent.h"
#include "Asset\SkeletonModel\Skeleton.h"
#include "Asset\SkeletonModel\SkeletonModel.h"
#include "Asset\Material.h"

class QSkeletonModelComponent;

class SkeletonModelComponentSubMesh : public IRenderable {
	friend class QSkeletonModelComponent;
public:
	SkeletonModelComponentSubMesh(QSkeletonModelComponent* model, const Asset::SkeletonModel::Mesh* mesh);
	~SkeletonModelComponentSubMesh();

	void updateMaterial();
	virtual void recreateResource() override;
	virtual void recreatePipeline() override;
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) override;
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) override;
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;

private:
	const Asset::SkeletonModel::Mesh* mMesh;
	QSkeletonModelComponent* mModel;
	Asset::Material* mMaterial = nullptr;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
};

class QSkeletonModelComponent :public IRenderableComponent {
	Q_OBJECT
		Q_COMPONENT(QSkeletonModelComponent)
		Q_PROPERTY(std::shared_ptr<Asset::SkeletonModel> SkeletonModel READ getSkeletonModel WRITE setSkeletonModel)
		Q_PROPERTY(QVector<std::shared_ptr<Asset::Material>> MaterialList READ getMaterialList WRITE setMaterialList)

		friend class SkeletonModelComponentSubMesh;
public:
	virtual void recreateResource() override;
	virtual void recreatePipeline() override;
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) override;
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) override;
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;

	std::shared_ptr<Asset::SkeletonModel> getSkeletonModel() const;
	void setSkeletonModel(std::shared_ptr<Asset::SkeletonModel> val);

	const QVector<std::shared_ptr<Asset::Material>>& getMaterialList() const;
	void setMaterialList(QVector<std::shared_ptr<Asset::Material>> val);
private:
	QVector<std::shared_ptr<Asset::Material>> mMaterialList;

	std::shared_ptr<Asset::Skeleton> mSkeleton;
	std::shared_ptr<Asset::SkeletonModel> mSkeletonModel;

	QRhiSPtr<QRhiBuffer> mBoneMatBuffer;
	QVector<std::shared_ptr<SkeletonModelComponentSubMesh>> mSkeletonSubMeshList;
};




#endif // QSkeletonModelComponent_h__
