#include "QSkeletonMeshComponent.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include "QFileInfo"
#include "QQueue"
#include "QDir"
#include "QSkeleton.h"
#include "Scene\Component\AssimpToolkit\Converter.h"
#include "QSkeletonAnimation.h"

QSkeletonMesh::QSkeletonMesh(QSkeletonModelComponent* model, aiMesh* mesh)
	:mModel(model)
{
	mVertices.resize(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		QSkeletonModelComponent::Vertex& vertex = mVertices[i];
		vertex.position = converter(mesh->mVertices[i]);
		if (mesh->mNormals)
			vertex.normal = converter(mesh->mNormals[i]);
		if (mesh->mTextureCoords[0]) {
			vertex.texCoord.setX(mesh->mTextureCoords[0][i].x);
			vertex.texCoord.setY(mesh->mTextureCoords[0][i].y);
		}
		if (mesh->mTangents)
			vertex.tangent = converter(mesh->mTangents[i]);
		if (mesh->mBitangents)
			vertex.bitangent = converter(mesh->mBitangents[i]);
	}
	mIndices.clear();
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			mIndices.push_back(face.mIndices[j]);
		}
	}
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
			auto bone = mModel->mSkeleton->addBoneNode(mesh->mBones[i]);
			int slot = 0;
			while (slot < std::size(mVertices[vertexId].boneIndex) && (mVertices[vertexId].boneWeight[slot] > 0.000001)) {
				slot++;
			}
			if (slot < std::size(mVertices[vertexId].boneIndex)) {
				mVertices[vertexId].boneIndex[slot] = bone->index;
				mVertices[vertexId].boneWeight[slot] = mesh->mBones[i]->mWeights[j].mWeight;;
			}
			else {
				qWarning("Lack of slot");
			}
		}
	}
}

void QSkeletonModelComponent::loadFromFile(const QString filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.toUtf8().constData(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (scene == nullptr)
		return;
	mMaterialList.resize(scene->mNumMaterials);
	for (uint i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		mMaterialList[i] = std::make_shared<QMaterial>();
		int diffuseCount = material->GetTextureCount(aiTextureType_DIFFUSE);
		for (int j = 0; j < diffuseCount; j++) {
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, j, &path);
			QString realPath = QFileInfo(filePath).dir().filePath(path.C_Str());
			QImage image;
			if (QFile::exists(realPath)) {
				image.load(realPath);
			}
			else {
				const aiTexture* embTexture = scene->GetEmbeddedTexture(path.C_Str());
				if (embTexture->mHeight == 0) {
					image.loadFromData((uchar*)embTexture->pcData, embTexture->mWidth, embTexture->achFormatHint);
				}
				else {
					image = QImage((uchar*)embTexture->pcData, embTexture->mWidth, embTexture->mHeight, QImage::Format_ARGB32);
				}
			}
			mMaterialList[i]->addTextureSampler("Diffuse", image);
		}
		if (diffuseCount) {
			mMaterialList[i]->setShadingCode("FragColor = texture(Diffuse,vUV); ");
		}
		else {
			mMaterialList[i]->setShadingCode("FragColor = vec4(1.0); ");
		}
		mMaterialList[i]->addRef(this);
	}
	mSkeleton.reset(new QSkeleton(this, scene));
	mMeshes.clear();
	QQueue<QPair<aiNode*, aiMatrix4x4>> qNode;
	qNode.push_back({ scene->mRootNode ,aiMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<aiNode*, aiMatrix4x4> node = qNode.takeFirst();
		for (unsigned int i = 0; i < node.first->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node.first->mMeshes[i]];
			std::shared_ptr<QSkeletonMesh> skeletonMesh = std::make_shared<QSkeletonMesh>(this, mesh);
			skeletonMesh->setMaterial(mMaterialList[mesh->mMaterialIndex]);
			mMeshes << skeletonMesh;
		}
		for (unsigned int i = 0; i < node.first->mNumChildren; i++) {
			qNode.push_back({ node.first->mChildren[i] ,node.second * node.first->mChildren[i]->mTransformation });
		}
	}
	mSkeleton->showDefaultPoses();
}

void QSkeletonModelComponent::tickEvent(float deltaSeconds) {
	if (mStatus.mCurrentAnimation) {
		mStatus.timeMs += deltaSeconds*1000.0f;
		if (mStatus.timeMs > mStatus.mCurrentAnimation->getDuration() && !mStatus.loop) {
			mStatus.timeMs = 0;
			mStatus.mCurrentAnimation = nullptr;
			return;
		}
		mStatus.mCurrentAnimation->show(mStatus.timeMs);
	}
}

bool QSkeletonModelComponent::playAnimationByIndex(int index, bool loop) {
	const auto& anims = mSkeleton->getAnimations();
	if (index >= 0 && index < anims.size()){
		playAnimation(anims[index], loop);
		return true;
	}
	return false;
}

bool QSkeletonModelComponent::playAnimation(std::shared_ptr<QSkeletonAnimation> anim, bool loop) {
	mStatus.mCurrentAnimation = anim;
	mStatus.timeMs = 0;
	mStatus.loop = loop;
	return true;
}
