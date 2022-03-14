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
	boneNode->index = mBoneMatrix.size();
	mBoneMatrix << boneNode->offsetMatrix.toGenericMatrix<4, 4>();
	mBoneMap[boneNode->name] = boneNode;
	return boneNode;
}

QVector<QSkeleton::QSkeleton::BoneMatrix> QSkeleton::getPosesMatrix() const
{
	QVector<QSkeleton::QSkeleton::BoneMatrix> matrix(mBoneMatrix.size());
	QQueue<QPair<std::shared_ptr<ModelNode>, QMatrix4x4>> qNode;
	qNode.push_back({ mRootNode ,QMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<std::shared_ptr<ModelNode>, QMatrix4x4> node = qNode.takeFirst();
		QMatrix4x4 globalMatrix = node.second * node.first->localMatrix;

		auto boneIter = mBoneMap.find(node.first->name);
		if (boneIter != mBoneMap.end()) {
			const int& index = (*boneIter)->index;
			matrix[index] = (globalMatrix * QMatrix4x4(mBoneMatrix[index])).toGenericMatrix<4, 4>();
		}

		for (unsigned int i = 0; i < node.first->children.size(); i++) {
			qNode.push_back({ node.first->children[i] ,globalMatrix });
		}
	}
	return matrix;
}

void QSkeleton::addAnimation(std::shared_ptr < QSkeletonAnimation > anim)
{
	mAnimations << anim;;
}