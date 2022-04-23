#ifndef IShapeComponent_h__
#define IShapeComponent_h__

#include "qvectornd.h"
#include "ECS\Component\RenderableComponent\IRenderableComponent.h"
#include "Asset\Material.h"

class IShapeComponent :public IRenderableComponent {
	Q_OBJECT
		Q_PROPERTY(std::shared_ptr<Asset::Material> Material READ getMaterial WRITE setMaterial)
public:
	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;
	};
	struct SimpleVertex {
		QVector3D position;
		QVector2D texCoord;
	};

	using Index = uint32_t;

	const QVector<IShapeComponent::Vertex>& getVertices() const { return mVertices; }
	void setVertices(const QVector<IShapeComponent::Vertex>&& val);

	const QVector<IShapeComponent::Index>& getIndices() const { return mIndices; }
	void setIndices(const QVector<IShapeComponent::Index>&& val);

	std::shared_ptr<Asset::Material> getMaterial() {
		return mMaterial;
	}
	void setSimpleVertex(const QVector<IShapeComponent::SimpleVertex>& val);

	void setMaterial(std::shared_ptr<Asset::Material> val);

	virtual void recreateResource() override;
	virtual void recreatePipeline() override;
	virtual void uploadResource(QRhiResourceUpdateBatch* batch) override;
	virtual void updatePrePass(QRhiCommandBuffer* cmdBuffer) override;
	virtual void updateResourcePrePass(QRhiResourceUpdateBatch* batch) override;
	virtual void renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) override;
	virtual bool isDefer() override;
private:
	QVector<Vertex> mVertices;
	QVector<Index> mIndices;

	std::shared_ptr<Asset::Material> mMaterial;

	QRhiSignal bNeedUpdateVertices;
	QRhiSignal bNeedUpdateIndices;

	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
	QRhiSPtr<QRhiShaderResourceBindings> mShaderResourceBindings;
};

#endif // IShapeComponent_h__