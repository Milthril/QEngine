#include "Skeleton.h"
#include "Serialization/QSerialization.h"
#include "QQueue"

void Asset::Skeleton::ModelNode::serialize(QDataStream& out) {
	out << name;
	out << localMatrix;
	out << children;
}

void Asset::Skeleton::ModelNode::deserialize(QDataStream& in) {
	in >> name;
	in >> localMatrix;
	in >> children;
}

void Asset::Skeleton::BoneNode::serialize(QDataStream& out) {
	out << name;
	out << index;
	out << offsetMatrix;
}

void Asset::Skeleton::BoneNode::deserialize(QDataStream& in) {
	in >> name;
	in >> index;
	in >> offsetMatrix;
}

void Asset::Skeleton::serialize(QDataStream& out) {
	IAsset::serialize(out);
	out << mRootNode;
	out << mBoneMap;
	out << mBoneOffsetMatrix;
	out << mCurrentPosesMatrix;
}

void Asset::Skeleton::deserialize(QDataStream& in) {
	IAsset::deserialize(in);
	in >> mRootNode;
	in >> mBoneMap;
	in >> mBoneOffsetMatrix;
	in >> mCurrentPosesMatrix;
	updateDefaultPoses();
}

void Asset::Skeleton::updateDefaultPoses() {
	QVector<Asset::Skeleton::Mat4> matrix(mBoneOffsetMatrix.size());
	QQueue<QPair<std::shared_ptr<ModelNode>, QMatrix4x4>> qNode;
	qNode.push_back({ mRootNode ,QMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<std::shared_ptr<ModelNode>, QMatrix4x4> node = qNode.takeFirst();
		QMatrix4x4 nodeMat = node.first->localMatrix;
		QMatrix4x4 globalMatrix = node.second * nodeMat;
		auto boneIter = mBoneMap.find(node.first->name);
		if (boneIter != mBoneMap.end()) {
			const int& index = (*boneIter)->index;
			matrix[index] = (globalMatrix * QMatrix4x4(mBoneOffsetMatrix[index])).toGenericMatrix<4, 4>();
		}

		for (unsigned int i = 0; i < node.first->children.size(); i++) {
			qNode.push_back({ node.first->children[i] ,globalMatrix });
		}
	}
	mCurrentPosesMatrix = matrix;
}

