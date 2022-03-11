#ifndef QDefaultProxyStaticMesh_h__
#define QDefaultProxyStaticMesh_h__

#include "QDefaultRenderer.h"

class QDefaultProxyStaticMesh :public QRhiProxy {
public:
	QDefaultProxyStaticMesh(std::shared_ptr<QStaticMeshComponent> shape);
private:
	std::shared_ptr<QStaticMeshComponent> mStaticMesh;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
protected:
	void recreateResource() override;
	void recreatePipeline(PipelineUsageFlags flags = PipelineUsageFlag::Normal) override;
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
};

#endif // QDefaultProxyStaticMesh_h__
