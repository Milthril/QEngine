#include "QSkeletonAnimation.h"
#include "QQueue"
#include "..\AssimpToolkit\MMDVmdParser.h"

QVector3D interp(const QVector3D& start, const QVector3D& end, double factor)
{
	Q_ASSERT(factor >= 0 && factor <= 1);
	return start + (end - start) * factor;
}

QSkeletonAnimation::QSkeletonAnimation(QSkeleton* skeleton)
	: mSkeleton(skeleton) {
}

void QSkeletonAnimation::show(const double& timeMs)
{
	QVector<QSkeleton::Mat4> matrix = calcBoneMatrix(timeMs);
	mSkeleton->setCurrentPosesMatrix(matrix);
}

void QSkeletonAnimation::loadFromAssimp(aiAnimation* anim)
{
	mDuration = anim->mDuration;
	mTicksPerSecond = anim->mTicksPerSecond;
	for (unsigned int i = 0; i < anim->mNumChannels; i++) {
		aiNodeAnim* node = anim->mChannels[i];
		QSkeletonAnimation::Node& skNode = mAnimNode[node->mNodeName.C_Str()];
		for (uint j = 0; j < node->mNumScalingKeys; j++) {
			const aiVectorKey& key = node->mScalingKeys[j];
			skNode.scaling[key.mTime] = QVector3D(key.mValue.x, key.mValue.y, key.mValue.z);
		}
		for (uint j = 0; j < node->mNumPositionKeys; j++) {
			const aiVectorKey& key = node->mPositionKeys[j];
			skNode.translation[key.mTime] = QVector3D(key.mValue.x, key.mValue.y, key.mValue.z);
		}
		for (uint j = 0; j < node->mNumRotationKeys; j++) {
			const aiQuatKey& key = node->mRotationKeys[j];
			skNode.rotation[key.mTime] = QQuaternion(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
		}
	}
}

QVector<QSkeleton::Mat4> QSkeletonAnimation::calcBoneMatrix(const double& timeMs)
{
	QVector<QSkeleton::QSkeleton::Mat4> matrix(mSkeleton->mBoneOffsetMatrix.size());
	QQueue<QPair<std::shared_ptr<QSkeleton::ModelNode>, QMatrix4x4>> qNode;
	qNode.push_back({ mSkeleton->mRootNode ,QMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<std::shared_ptr<QSkeleton::ModelNode>, QMatrix4x4> node = qNode.takeFirst();
		QMatrix4x4 nodeMat = node.first->localMatrix;

		//auto animNodeIter = mAnimNode.find(node.first->name);
		//if (animNodeIter != mAnimNode.end()) {
		//	nodeMat = animNodeIter->getMatrix(timeMs);
		//}
		QMatrix4x4 globalMatrix = node.second * nodeMat;
		auto boneIter = mSkeleton->mBoneMap.find(node.first->name);
		if (boneIter != mSkeleton->mBoneMap.end()) {
			const int& index = (*boneIter)->index;
			matrix[index] = (globalMatrix * QMatrix4x4(mSkeleton->mBoneOffsetMatrix[index])).toGenericMatrix<4, 4>();
		}
		for (unsigned int i = 0; i < node.first->children.size(); i++) {
			qNode.push_back({ node.first->children[i] ,globalMatrix });
		}
	}
	return matrix;
}

QMatrix4x4 QSkeletonAnimation::Node::getMatrix(const double& timeMs)
{
	QMatrix4x4 mat;
	//移动插值
	auto endTranslation = translation.upperBound(timeMs);
	auto startTranslation = endTranslation == translation.begin() ? endTranslation : endTranslation - 1;
	if (endTranslation != translation.end()) {
		double deltaTime = endTranslation.key() - startTranslation.key();
		double factor = deltaTime == 0 ? 0 : (timeMs - startTranslation.key()) / deltaTime;
		mat.translate(interp(startTranslation.value(), endTranslation.value(), factor));
	}
	else
		mat.translate(startTranslation.value());
	//旋转插值
	auto endRotation = rotation.upperBound(timeMs);
	auto startRotation = endRotation == rotation.begin() ? endRotation : endRotation - 1;
	if (endRotation != rotation.end()) {
		double deltaTime = endRotation.key() - startRotation.key();
		double factor = deltaTime == 0 ? 0 : (timeMs - startRotation.key()) / deltaTime;
		mat.rotate(QQuaternion::nlerp(startRotation.value(), endRotation.value(), factor).normalized());
	}
	else
		mat.rotate(startRotation.value().normalized());
	//缩放插值
	auto endScaling = scaling.upperBound(timeMs);
	auto startScaling = endScaling == scaling.begin() ? endScaling : endScaling - 1;
	if (endScaling != scaling.end()) {
		double deltaTime = endScaling.key() - startScaling.key();
		double factor = deltaTime == 0 ? 0 : (timeMs - startScaling.key()) / deltaTime;
		mat.scale(interp(startScaling.value(), endScaling.value(), factor));
	}
	else
		mat.scale(startScaling.value());
	return mat;
}