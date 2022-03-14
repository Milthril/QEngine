#ifndef QSkeleton_h__
#define QSkeleton_h__
#include <QString>
#include <memory>
#include "QMatrix4x4"

class QSkeletonModelComponent;
class QSkeletonAnimation;
struct aiNode;
struct aiScene;
struct aiBone;

class QSkeleton {
	friend class QSkeletonAnimation;
public:
	QSkeleton(QSkeletonModelComponent* model, const aiScene* scene);
	struct ModelNode {
		QString name;
		QMatrix4x4 localMatrix;
		QVector<std::shared_ptr<ModelNode>> children;
	};

	struct BoneNode {
		QString name;
		uint16_t index;
		QMatrix4x4 offsetMatrix;
	};

	using BoneMatrix = QGenericMatrix<4, 4, float>;
	std::shared_ptr<BoneNode> getBoneNode(const QString& name);
	std::shared_ptr<BoneNode> addBoneNode(aiBone* bone);
	const QVector<QSkeleton::BoneMatrix>& getBoneMatrix() const { return mBoneMatrix; }
	QVector<QSkeleton::BoneMatrix> getPosesMatrix() const;
	void addAnimation(std::shared_ptr < QSkeletonAnimation > anim);
private:
	std::shared_ptr<ModelNode> processModelNode(aiNode* node);
	void resolveAnimations(const aiScene* scene);
private:
	QSkeletonModelComponent* mModel;
	QVector<std::shared_ptr<QSkeletonAnimation>> mAnimations;
	std::shared_ptr<ModelNode> mRootNode;;
	QHash<QString, std::shared_ptr<BoneNode>> mBoneMap;
	QVector<BoneMatrix> mBoneMatrix;
};

#endif // QSkeleton_h__
