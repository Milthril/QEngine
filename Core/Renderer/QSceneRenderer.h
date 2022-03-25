﻿#ifndef QSceneRenderer_h__
#define QSceneRenderer_h__

#include "Scene/QScene.h"
#include "CommonPass/BloomPass.h"
#include "RHI/QRhiDefine.h"

class QPrimitiveComponent;
class QStaticMeshComponent;
class QStaticMeshComponent;
class QSkeletonModelComponent;
class QParticleComponent;
class QSkyBoxComponent;
class QSceneRenderer;
class DebugDrawPass;

class QRhiProxy {
public:
	virtual void recreateResource() {}
	virtual void recreatePipeline() {}
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) {}
	virtual void updateResource(QRhiResourceUpdateBatch* batch) {}
	virtual void updatePrePass(QRhiCommandBuffer* cmdBuffer) {}
	virtual void drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {}
public:
	QSceneRenderer* mRenderer;
	std::shared_ptr<QSceneComponent> mComponent;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
};

class QSceneRenderer :public QObject {
	Q_OBJECT
public:
	QSceneRenderer();
	void setScene(std::shared_ptr<QScene> scene);
	std::shared_ptr<QScene> getScene() { return mScene; }

	int getSampleCount() const { return mSampleCount; }
	void setSampleCount(int val) { mSampleCount = val; }

	QRhiSPtr<QRhiRenderPassDescriptor> getRootRenderPassDescriptor() const { return mRootRenderPassDescriptor; }
	void setRootRenderPassDescriptor(QRhiSPtr<QRhiRenderPassDescriptor> val) { mRootRenderPassDescriptor = val; }

	virtual QRhiSPtr<QRhiRenderPassDescriptor> getRenderPassDescriptor() const { return getRootRenderPassDescriptor(); }

	void renderInternal(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget);

	virtual void render(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget) = 0;

	QMatrix4x4 getViewMatrix();
	QMatrix4x4 getClipMatrix() const;
	QMatrix4x4 getVP();

	std::shared_ptr<QRhiProxy> createPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component);

	static QShader createShaderFromCode(QShader::Stage stage, const char* code);

	bool debugEnabled() const;
	void setDegbuPainter(std::shared_ptr<DebugDrawPass> painter);

	virtual QVector<QRhiGraphicsPipeline::TargetBlend> getDefaultBlends() { return{}; }
	virtual void requestReadbackCompId(const QPoint& screenPt) {}

Q_SIGNALS:
	void readBackCompId(QSceneComponent::ComponentId);
private:
	void onPrimitiveInserted(uint32_t index, std::shared_ptr<QPrimitiveComponent> primitive);
	void onPrimitiveRemoved(std::shared_ptr<QPrimitiveComponent> primitive);
	void onLightChanged();
	void onSkyBoxChanged();
	void tryResetUniformProxy();
	void tryResetPrimitiveProxy(QRhiResourceUpdateBatch* batch);
	void tryResetSkyBox(QRhiResourceUpdateBatch* batch);
	void resetPrimitiveProxy(std::shared_ptr<QPrimitiveComponent> component);

	virtual std::shared_ptr<QRhiProxy> createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createParticleProxy(std::shared_ptr<QParticleComponent>) = 0;
	virtual std::shared_ptr<QRhiProxy> createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent>) = 0;

protected:
	std::shared_ptr<QScene> mScene;
	int mSampleCount;
	QRhiSPtr<QRhiRenderPassDescriptor> mRootRenderPassDescriptor;

	QHash<QSceneComponent::ComponentId, std::shared_ptr<QRhiProxy>> mPrimitiveProxyMap;	//图元组件代理
	QList<std::shared_ptr<QRhiProxy>> mProxyUploadList;
	std::shared_ptr<QRhiProxy> mSkyBoxProxy;
	std::shared_ptr<DebugDrawPass> mDebugPainter;
};

#endif // QSceneRenderer_h__
