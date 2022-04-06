#ifndef QDefaultProxySkeletonModel_h__
#define QDefaultProxySkeletonModel_h__

#include "QDefaultRenderer.h"
#include "Renderer\IRhiProxy.h"

class QSkeletonMesh;
class QSkeletonModelComponent;

class QDefaultProxySkeletonModel :public IRhiProxy {
public:
	QDefaultProxySkeletonModel(std::shared_ptr<QSkeletonModelComponent> shape);
private:
	struct SkeletonMeshProxy {
		int vertexOffset = 0;
		int vertexRange = 0;
		int indexOffset = 0;
		int indexRange = 0;
		QRhiSPtr<QRhiGraphicsPipeline> pipeline;
		QRhiSPtr<QRhiShaderResourceBindings> shaderBindings;
		std::shared_ptr<QSkeletonMesh> mesh;
	};

	QVector<std::shared_ptr<SkeletonMeshProxy>> mMeshProxyList;
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
