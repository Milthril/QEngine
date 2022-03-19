#ifndef QDefaultProxyStaticMesh_h__
#define QDefaultProxyStaticMesh_h__

#include "QDefaultRenderer.h"

class QDefaultProxyStaticMesh :public QRhiProxy {

public:
	QDefaultProxyStaticMesh(std::shared_ptr<QStaticMeshComponent> shape,bool usedByParticle = false);
private:
	std::shared_ptr<QStaticMeshComponent> mStaticMesh;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
public:
	void recreateResource() override;
	void recreatePipeline() override;
	void uploadResource(QRhiResourceUpdateBatch* batch) override;
	void updateResource(QRhiResourceUpdateBatch* batch) override;
	void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
private:
	bool bUsedByParticle = false;
};

#endif // QDefaultProxyStaticMesh_h__
