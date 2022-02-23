#ifndef QSceneRenderer_h__
#define QSceneRenderer_h__

#include "Render/Scene/QScene.h"
#include <memory>

class QPrimitiveComponent;
class QShapeComponent;
class QStaticMeshComponent;
class QSkeletonMeshComponent;
class QText2DComponent;
class QParticleComponent;
class QSkyBoxComponent;
class QSceneRenderer;

class QRhiProxy {
public:
	virtual void recreateResource() {}
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) {}
	virtual void updateResource(QRhiResourceUpdateBatch* batch) {}
	virtual void draw(QRhiCommandBuffer* cmdBuffer) {}
public:
	QSceneRenderer* mRenderer;
	std::shared_ptr<QRhiGraphicsPipeline> mPipeline;
	std::shared_ptr<QRhi> mRhi;
	std::shared_ptr<QSceneComponent> mComponent;
};

class QSceneRenderer :public QObject {
	Q_OBJECT
public:
	QSceneRenderer(std::shared_ptr<QRhi> rhi, std::shared_ptr<QRhiRenderPassDescriptor> renderPassDescriptor);
	void setScene(std::shared_ptr<QScene> scene);
	std::shared_ptr<QScene> getScene() { return mScene; }
	std::shared_ptr<QRhiRenderPassDescriptor> RenderPassDescriptor() const { return mRenderPassDescriptor; }
	std::shared_ptr<QRhi> Rhi() const { return mRhi; }

	virtual void setRenderTargetSize(QSize size);
	void renderInternal(QRhiCommandBuffer* buffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch);
	virtual void render(QRhiCommandBuffer* buffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch) = 0;
	QMatrix4x4 getViewMatrix();
	QMatrix4x4 getClipMatrix() const;
	QMatrix4x4 getVP();
	void setClipMatrix(QMatrix4x4 val);
private:
	void onPrimitiveInserted(uint32_t index, std::shared_ptr<QPrimitiveComponent> primitive);
	void onPrimitiveRemoved(std::shared_ptr<QPrimitiveComponent> primitive);
	void onLightChanged();
protected:
	std::shared_ptr<QRhiProxy> createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component);
	void resetPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component);
	virtual std::shared_ptr<QRhiProxy> createShapeProxy(std::shared_ptr<QShapeComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonMeshComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createText2DProxy(std::shared_ptr<QText2DComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) = 0;
protected:
	std::shared_ptr<QRhi> mRhi;
	std::shared_ptr<QRhiRenderPassDescriptor> mRenderPassDescriptor;
	std::shared_ptr<QScene> mScene;
	QHash<QSceneComponent::ComponentId, std::shared_ptr<QRhiProxy>> mProxyMap;
	QList<std::shared_ptr<QRhiProxy>> mProxyUploadList;
	QMatrix4x4 mClipMatrix;
	QSize mRTSize;
};

#endif // QSceneRenderer_h__
