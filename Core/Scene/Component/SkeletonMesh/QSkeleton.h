#ifndef QSkeleton_h__
#define QSkeleton_h__
#include <memory>
#include "QMatrix4x4"
#include <QString>

struct aiNode;
class QSkeletonModelComponent;

class QSkeleton {
public:
	QSkeleton(QSkeletonModelComponent* model, aiNode* rootNode);

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
	std::shared_ptr<BoneNode> addBoneNode(aiBone *bone);
	const QVector<QSkeleton::BoneMatrix>& getBoneMatrix() const { return mBoneMatrix; }
	QVector<QSkeleton::BoneMatrix> getPosesMatrix() const;
private:
	std::shared_ptr<ModelNode> processModelNode(aiNode* node);
private:
	QSkeletonModelComponent* mModel;
	std::shared_ptr<ModelNode> mRootNode;;
	QHash<QString, std::shared_ptr<BoneNode>> mBoneMap;
	QVector<BoneMatrix> mBoneMatrix;
};

#endif // QSkeleton_h__
