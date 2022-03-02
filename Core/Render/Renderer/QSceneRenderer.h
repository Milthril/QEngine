#ifndef QSceneRenderer_h__
#define QSceneRenderer_h__

#include "Render/Scene/QScene.h"
#include <memory>

class QPrimitiveComponent;
class QShapeComponent;
class QStaticMeshComponent;
class QSkeletonMeshComponent;
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

template<typename _Ty>
class QRhiSPtr :public std::shared_ptr<_Ty> {
public:
	void reset(_Ty* res) noexcept {
		std::shared_ptr<_Ty>::reset(res, [](_Ty* res) {
			res->destroy();
		});
	}
};

class QSceneRenderer :public QObject {
	Q_OBJECT
public:
	QSceneRenderer(std::shared_ptr<QRhi> rhi, int sampleCount, QRhiSPtr<QRhiRenderPassDescriptor> renderPassDescriptor);
	void setScene(std::shared_ptr<QScene> scene);

	std::shared_ptr<QScene> getScene() { return mScene; }
	virtual QRhiSPtr<QRhiRenderPassDescriptor> getRenderPassDescriptor() const { return mRootRenderPassDescriptor; }
	std::shared_ptr<QRhi> getRhi() const { return mRhi; }

	virtual void setRenderTargetSize(QSize size);
	void renderInternal(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch);
	virtual void render(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget, QRhiResourceUpdateBatch* batch) = 0;
	QMatrix4x4 getViewMatrix();
	QMatrix4x4 getClipMatrix() const;
	QMatrix4x4 getVP();
	void setClipMatrix(QMatrix4x4 val);
	int getSampleCount() const { return mSampleCount; }

	static QShader createShaderFromCode(QShader::Stage stage, const char* code);
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
	virtual std::shared_ptr<QRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) = 0;
protected:
	std::shared_ptr<QRhi> mRhi;
	std::shared_ptr<QScene> mScene;
	QRhiSPtr<QRhiRenderPassDescriptor> mRootRenderPassDescriptor;
	QHash<QSceneComponent::ComponentId, std::shared_ptr<QRhiProxy>> mProxyMap;
	QList<std::shared_ptr<QRhiProxy>> mProxyUploadList;
	QMatrix4x4 mClipMatrix;
	QSize mRTSize;
	int mSampleCount;
};

#endif // QSceneRenderer_h__
