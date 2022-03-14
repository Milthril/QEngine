#include "assimp\scene.h"
#include "QSkeleton.h"
#include "QSkeletonMeshComponent.h"
#include "Scene\Component\AssimpToolkit\Converter.h"
#include "QQueue"
#include "QSkeletonAnimation.h"

QSkeleton::QSkeleton(QSkeletonModelComponent* model, const aiScene* scene)
	: mModel(model)
	, mRootNode(processModelNode(scene->mRootNode))
{
	resolveAnimations(scene);
}

std::shared_ptr<QSkeleton::ModelNode> QSkeleton::processModelNode(aiNode* node)
{
	std::shared_ptr<QSkeleton::ModelNode> boneNode = std::make_shared<QSkeleton::ModelNode>();
	boneNode->name = node->mName.C_Str();
	boneNode->localMatrix = converter(node->mTransformation);
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		boneNode->children.push_back(processModelNode(node->mChildren[i]));
	}
	return boneNode;
}

void QSkeleton::resolveAnimations(const aiScene* scene)
{
	for (int i = 0; i < scene->mNumAnimations; i++) {
		std::shared_ptr<QSkeletonAnimation> anim = std::make_shared<QSkeletonAnimation>(this);
		anim->loadFromAssimp(scene->mAnimations[i]);
		addAnimation(anim);
	}
}

void QSkeleton::showDefaultPoses()
{
	QVector<QSkeleton::QSkeleton::Mat4> matrix(mBoneOffsetMatrix.size());
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
	setCurrentPosesMatrix(matrix);
}

QVector<QSkeleton::QSkeleton::Mat4> QSkeleton::getCurrentPosesMatrix() const
{
	return mCurrentPosesMatrix;
}

void QSkeleton::setCurrentPosesMatrix(QVector<Mat4> val)
{
	mCurrentPosesMatrix = val;
}

std::shared_ptr<QSkeleton::BoneNode> QSkeleton::getBoneNode(const QString& name)
{
	return mBoneMap.value(name);
}

std::shared_ptr<QSkeleton::BoneNode> QSkeleton::addBoneNode(aiBone* bone) {
	if (mBoneMap.contains(bone->mName.C_Str()))
		return mBoneMap[bone->mName.C_Str()];
	std::shared_ptr<QSkeleton::BoneNode> boneNode = std::make_shared<QSkeleton::BoneNode>();
	boneNode->name = bone->mName.C_Str();
	qDebug() << boneNode->name;
	boneNode->offsetMatrix = converter(bone->mOffsetMatrix);
	boneNode->index = mBoneOffsetMatrix.size();
	mBoneOffsetMatrix << boneNode->offsetMatrix.toGenericMatrix<4, 4>();
	mBoneMap[boneNode->name] = boneNode;
	return boneNode;
}

void QSkeleton::addAnimation(std::shared_ptr < QSkeletonAnimation > anim)
{
	mAnimations << anim;
}