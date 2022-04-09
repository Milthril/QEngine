#ifndef QDefaultProxySkeletonModel_h__
#define QDefaultProxySkeletonModel_h__

#include "QDefaultRenderer.h"
#include "Renderer\IRhiProxy.h"

class QSkeletonMesh;
class QSkeletonModelComponent;
class QDefaultProxySkeletonModel;

class QDefaultProxySkeletonMesh : public IRhiProxy {
	friend class QDefaultProxySkeletonModel;
private:
	int mVertexOffset = 0;
	int mVertexRange = 0;
	int mIndexOffset = 0;
	int mIndexRange = 0;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderBindings;
	std::shared_ptr<QSkeletonMesh> mMesh;
	QDefaultProxySkeletonModel* mModel;
public:
	void recreateResource() override;
	void recreatePipeline() override;
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
};

class QDefaultProxySkeletonModel :public IRhiProxy {
	friend class QDefaultProxySkeletonMesh;
public:
	QDefaultProxySkeletonModel(std::shared_ptr<QSkeletonModelComponent> shape);
private:
	QVector<std::shared_ptr<QDefaultProxySkeletonMesh>> mMeshProxyList;
	std::shared_ptr<QSkeletonModelComponent> mSkeletonModel;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
protected:
	void recreateResource() override;
	void recreatePipeline() override;
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
};

#endif // QDefaultProxySkeletonModel_h__
