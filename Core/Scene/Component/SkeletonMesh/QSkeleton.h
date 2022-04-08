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
	using Mat4 = QGenericMatrix<4, 4, float>;
	void showDefaultPoses();
	QVector<QSkeleton::Mat4> getCurrentPosesMatrix() const;
	void setCurrentPosesMatrix(QVector<QSkeleton::Mat4> val);

	std::shared_ptr<BoneNode> getBoneNode(const QString& name);
	std::shared_ptr<BoneNode> addBoneNode(aiBone* bone);
	const QVector<QSkeleton::Mat4>& getBoneMatrix() const { return mBoneOffsetMatrix; }
	void addAnimation(std::shared_ptr < QSkeletonAnimation > anim);
	const QVector<std::shared_ptr<QSkeletonAnimation>>& getAnimations() const { return mAnimations; }
private:
	std::shared_ptr<ModelNode> processModelNode(aiNode* node);
	void resolveAnimations(const aiScene* scene);
private:
	QSkeletonModelComponent* mModel;
	QVector<std::shared_ptr<QSkeletonAnimation>> mAnimations;
	std::shared_ptr<ModelNode> mRootNode;;
	QHash<QString, std::shared_ptr<BoneNode>> mBoneMap;
	QVector<Mat4> mBoneOffsetMatrix;
	QVector<Mat4> mCurrentPosesMatrix;
};

#endif // QSkeleton_h__
