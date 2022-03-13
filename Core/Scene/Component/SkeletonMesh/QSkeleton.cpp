#include "assimp\scene.h"
#include "QSkeleton.h"
#include "QSkeletonMeshComponent.h"
#include "Scene\Component\AssimpToolkit.h"

QSkeleton::QSkeleton(QSkeletonModelComponent* model, aiNode* rootNode)
	: mModel(model)
{
	mBoneRoot = processBoneNode(rootNode);
}

std::shared_ptr<QSkeleton::BoneNode> QSkeleton::getBoneNode(const QString& name)
{
	return mBoneMap.value(name);
}

std::shared_ptr<QSkeleton::BoneNode> QSkeleton::processBoneNode(aiNode* node)
{
	std::shared_ptr<QSkeleton::BoneNode> boneNode = std::make_shared<QSkeleton::BoneNode>();
	boneNode->name = node->mName.C_Str();
	boneNode->localMatrix = converter(node->mTransformation);
	boneNode->index = mBoneMatrix.size();
	mBoneMap[boneNode->name] = boneNode;
	mBoneMatrix << BoneMatrix();
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		boneNode->children.push_back(processBoneNode(node->mChildren[i]));
	}
	return boneNode;
}
