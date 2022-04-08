#ifndef QSkeletonMeshComponent_h__
#define QSkeletonMeshComponent_h__

#include "Scene\Component\QPrimitiveComponent.h"
#include "assimp\mesh.h"
#include "Scene\Material\QMaterial.h"
#include "QSkeleton.h"
#include "EventHandler\QTickEventHandler.h"

class QSkeletonMesh;

class QSkeletonModelComponent :public QPrimitiveComponent ,public QTickEventHandler{
	friend class QSkeletonMesh;
	Q_OBJECT
	Q_PROPERTY(QVector<std::shared_ptr<QMaterial>> MaterialList READ getMaterialList WRITE setMaterialList);
public:
	void loadFromFile(const QString filePath);
	QSceneComponent::ProxyType type() override {
		return QSceneComponent::SkeletonMesh;
	}
	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;
		uint32_t boneIndex[4] = { 0,0,0,0 };
		QVector4D boneWeight;
	};
	const QVector<std::shared_ptr<QSkeletonMesh>>& getMeshes() const { return mMeshes; }
	std::shared_ptr<QSkeleton> getSkeleton() const { return mSkeleton; }
	const QVector<std::shared_ptr<QSkeletonAnimation>>& getAnimations() const { return mSkeleton->getAnimations(); }

	const QVector<std::shared_ptr<QMaterial>>& getMaterialList() const { return mMaterialList; }
	void setMaterialList(QVector<std::shared_ptr<QMaterial>> val) { mMaterialList = val; }

	void tickEvent(float deltaSeconds) override;
	bool playAnimationByIndex(int index,bool loop = false);
	bool playAnimation(std::shared_ptr<QSkeletonAnimation> anim,bool loop = false);
protected:
	struct Status {
		double timeMs = 0;
		std::shared_ptr <QSkeletonAnimation> mCurrentAnimation;
		bool loop = false;
	}mStatus;

	std::shared_ptr<QSkeleton> mSkeleton;
	QVector<std::shared_ptr<QSkeletonMesh>> mMeshes;
	QVector<std::shared_ptr<QMaterial>> mMaterialList;
};


class QSkeletonMesh{
public:
	QSkeletonMesh(QSkeletonModelComponent* model, aiMesh* mesh);
	std::shared_ptr<QMaterial> getMaterial() const { return mMaterial; }
	void setMaterial(std::shared_ptr<QMaterial> val) { mMaterial = val; }
	const QVector<QSkeletonModelComponent::Vertex>& getVertices() const { return mVertices; }
	const QVector<QSkeletonModelComponent::Index>& getIndices() const { return mIndices; }
private:
	QSkeletonModelComponent* mModel;
	QVector<QSkeletonModelComponent::Vertex> mVertices;
	QVector<QSkeletonModelComponent::Index> mIndices;
	std::shared_ptr<QMaterial> mMaterial;
};

#endif // QSkeletonMeshComponent_h__
