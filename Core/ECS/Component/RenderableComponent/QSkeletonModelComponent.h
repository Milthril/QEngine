#ifndef QSkeletonModelComponent_h__
#define QSkeletonModelComponent_h__

#include "IRenderableComponent.h"
#include "Asset\SkeletonModel\Skeleton.h"
#include "Asset\SkeletonModel\SkeletonModel.h"
#include "Asset\SkeletonModel\SkeletonAnimation.h"
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
		Q_PROPERTY(std::shared_ptr<Asset::SkeletonAnimation> Animation READ getAnimation WRITE setAnimation)

		friend class SkeletonModelComponentSubMesh;
public:
	virtual void recreateResource() override;
	virtual void recreatePipeline() override;
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) override;
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) override;
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;

	std::shared_ptr<Asset::SkeletonModel> getSkeletonModel() const;
	void setSkeletonModel(std::shared_ptr<Asset::SkeletonModel> val);

	std::shared_ptr<Asset::SkeletonAnimation> getAnimation() const;
	void setAnimation(std::shared_ptr<Asset::SkeletonAnimation> val);

	const QVector<std::shared_ptr<Asset::Material>>& getMaterialList() const;
	void setMaterialList(QVector<std::shared_ptr<Asset::Material>> val);

	std::shared_ptr<Asset::Skeleton> getSkeleton() const { return mSkeleton; }

	virtual bool isDefer() override;
private:
	std::shared_ptr<Asset::Skeleton> mSkeleton;
	std::shared_ptr<Asset::SkeletonModel> mSkeletonModel;

	std::shared_ptr<Asset::SkeletonAnimation> mAnimation;

	QVector<std::shared_ptr<Asset::Material>> mMaterialList;
	QRhiSPtr<QRhiBuffer> mBoneMatBuffer;
	QVector<std::shared_ptr<SkeletonModelComponentSubMesh>> mSkeletonSubMeshList;
};




#endif // QSkeletonModelComponent_h__
