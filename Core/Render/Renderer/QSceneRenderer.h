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

class QRhiProxy {
public:
	virtual void updateResource(QRhiResourceUpdateBatch* batch, QMatrix4x4 VP) {}
	virtual void draw(QRhiCommandBuffer* cmdBuffer) {}
public:
	std::shared_ptr<QRhiGraphicsPipeline> mPipeline;
};

class QSceneRenderer :public QObject {
	Q_OBJECT
public:
	QSceneRenderer(std::shared_ptr<QRhi> rhi, std::shared_ptr<QRhiRenderPassDescriptor> renderPassDescriptor) :mRhi(rhi), mRenderPassDescriptor(renderPassDescriptor) {}
	void setScene(std::shared_ptr<QScene> scene);
	std::shared_ptr<QScene> getScene() { return mScene; }
	std::shared_ptr<QRhiRenderPassDescriptor> RenderPassDescriptor() const { return mRenderPassDescriptor; }
	std::shared_ptr<QRhi> Rhi() const { return mRhi; }

	virtual void render(QRhiCommandBuffer* buffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch) = 0;
	QMatrix4x4 getViewMatrix();
	QMatrix4x4 getClipMatrix() const;
	void setClipMatrix(QMatrix4x4 val);
private:
	void onPrimitiveInserted(uint32_t index, std::shared_ptr<QPrimitiveComponent> primitive);
	void onPrimitiveRemoved(std::shared_ptr<QPrimitiveComponent> primitive);
	void onLightChanged();
protected:
	std::shared_ptr<QRhiProxy> createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component);
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
	QHash<QSceneComponent::Type, std::shared_ptr<QRhiGraphicsPipeline>> mGraphicsPipelineMap;
	QHash<QSceneComponent::ComponentId, std::shared_ptr<QRhiProxy>> mProxyMap;
	QMatrix4x4 mClipMatrix;
};

#endif // QSceneRenderer_h__
