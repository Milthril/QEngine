#ifndef Skeleton_h__
#define Skeleton_h__

#include "Asset\IAsset.h"
#include "QGenericMatrix"
#include "QMatrix4x4"

namespace Asset {

class Skeleton : public IAsset {
public:
	using Mat4 = QGenericMatrix<4, 4, float>;

	struct ModelNode {
		QString name;
		QMatrix4x4 localMatrix;
		QVector<std::shared_ptr<ModelNode>> children;

		void serialize(QDataStream& out);
		void deserialize(QDataStream& in);
	};

	struct BoneNode {
		QString name;
		uint16_t index;
		QMatrix4x4 offsetMatrix;

		void serialize(QDataStream& out);
		void deserialize(QDataStream& in);
	};

	std::shared_ptr<Asset::Skeleton::ModelNode> getRootNode() const { return mRootNode; }
	void setRootNode(std::shared_ptr<Asset::Skeleton::ModelNode> val) { mRootNode = val; }

	const QHash<QString, std::shared_ptr<Asset::Skeleton::BoneNode>>& getBoneMap() const { return mBoneMap; }
	void setBoneMap(QHash<QString, std::shared_ptr<Asset::Skeleton::BoneNode>>&& val) { mBoneMap = val; }

	const QVector<Asset::Skeleton::Mat4>& getBoneOffsetMatrix() const { return mBoneOffsetMatrix; }
	void setBoneOffsetMatrix(QVector<Asset::Skeleton::Mat4>&& val) { mBoneOffsetMatrix = val; }

	const QVector<Asset::Skeleton::Mat4>& getCurrentPosesMatrix() const { return mCurrentPosesMatrix; }
	void setCurrentPosesMatrix(QVector<Asset::Skeleton::Mat4>&& val) { mCurrentPosesMatrix = val; }

	void serialize(QDataStream& out) override;
	void deserialize(QDataStream& in) override;

	virtual IAsset::Type type() override {
		return IAsset::Skeleton;
	}
	void updateDefaultPoses();

private:
	std::shared_ptr<ModelNode> mRootNode;
	QHash<QString, std::shared_ptr<BoneNode>> mBoneMap;
	QVector<Mat4> mBoneOffsetMatrix;
	QVector<Mat4> mCurrentPosesMatrix;

};
}

Q_DECLARE_BUILTIN_METATYPE(ModelNode,110000, Asset::Skeleton::ModelNode);
Q_DECLARE_BUILTIN_METATYPE(BoneNode, 110001, Asset::Skeleton::BoneNode);

Q_ENGINE_DECLARE_ASSET_TYPE(Asset::Skeleton, IAsset::Skeleton);

#endif // Skeleton_h__
