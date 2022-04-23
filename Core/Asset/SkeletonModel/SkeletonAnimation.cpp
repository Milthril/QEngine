#include "SkeletonAnimation.h"
#include "Serialization/QSerialization.h"
#include "QQueue"

QVector3D interp(const QVector3D& start, const QVector3D& end, double factor) {
	Q_ASSERT(factor >= 0 && factor <= 1);
	return start + (end - start) * factor;
}

QDataStream& operator<<(QDataStream& out, const Asset::SkeletonAnimation::Node& anim) {
	out << anim.translation << anim.rotation << anim.scaling;
	return out;
}

QDataStream& operator>>(QDataStream& in, Asset::SkeletonAnimation::Node& anim) {
	in >> anim.translation >> anim.rotation >> anim.scaling;
	return in;
}

void Asset::SkeletonAnimation::serialize(QDataStream& out) {
	IAsset::serialize(out);
	out << mSkeletonSHA;
	out << mAnimNode;
	out << mDuration;
	out << mTicksPerSecond;
}

void Asset::SkeletonAnimation::deserialize(QDataStream& in) {
	IAsset::deserialize(in);
	in >> mSkeletonSHA;
	in >> mAnimNode;
	in >> mDuration;
	in >> mTicksPerSecond;
}

void  Asset::SkeletonAnimation::play(Asset::Skeleton* skeleton,double timeMs) {
	timeMs = fmod(timeMs, mDuration/ mTicksPerSecond);

	QVector<Skeleton::Mat4> matrix(skeleton->getBoneOffsetMatrix().size());
	QQueue<QPair<std::shared_ptr<Skeleton::ModelNode>, QMatrix4x4>> qNode;
	qNode.push_back({ skeleton->getRootNode() ,QMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<std::shared_ptr<Skeleton::ModelNode>, QMatrix4x4> node = qNode.takeFirst();
		QMatrix4x4 nodeMat = node.first->localMatrix;

		auto animNodeIter = mAnimNode.find(node.first->name);
		if (animNodeIter != mAnimNode.end()) {
			nodeMat = animNodeIter->getMatrix(timeMs);
		}
		QMatrix4x4 globalMatrix = node.second * nodeMat;
		auto boneIter = skeleton->getBoneMap().find(node.first->name);
		if (boneIter != skeleton->getBoneMap().end()) {
			const int& index = (*boneIter)->index;
			matrix[index] = (globalMatrix * QMatrix4x4(skeleton->getBoneOffsetMatrix()[index])).toGenericMatrix<4, 4>();
		}
		for (unsigned int i = 0; i < node.first->children.size(); i++) {
			qNode.push_back({ node.first->children[i] ,globalMatrix });
		}
	}
	skeleton->setCurrentPosesMatrix(std::move(matrix));
}

QMatrix4x4 Asset::SkeletonAnimation::Node::getMatrix(const double& timeMs) {
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
