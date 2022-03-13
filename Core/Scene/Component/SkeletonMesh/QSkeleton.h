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
	struct BoneNode {
		QString name;
		uint16_t index;
		QMatrix4x4 localMatrix;
		QMatrix4x4 offsetMatrix;
		QVector<std::shared_ptr<BoneNode>> children;
	};	
	using BoneMatrix = QGenericMatrix<4, 4, float>;
	std::shared_ptr<BoneNode> getBoneNode(const QString& name);
	const QVector<QSkeleton::BoneMatrix>& getBoneMatrix() const { return mBoneMatrix; }
private:
	std::shared_ptr<QSkeleton::BoneNode> processBoneNode(aiNode* node);
private:
	QSkeletonModelComponent* mModel;
	std::shared_ptr<BoneNode> mBoneRoot;;
	QHash<QString, std::shared_ptr<BoneNode>> mBoneMap;
	QVector<BoneMatrix> mBoneMatrix;
};

#endif // QSkeleton_h__
