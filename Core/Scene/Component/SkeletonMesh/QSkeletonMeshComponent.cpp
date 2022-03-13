#include "QSkeletonMeshComponent.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include "QFileInfo"
#include "QQueue"
#include "QDir"
#include "QSkeleton.h"
#include "Scene\Component\AssimpToolkit.h"

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
			int boneIndex = 0;
			auto bone = mModel->getBoneNode(mesh->mBones[i]->mName.C_Str());
			if (bone) {
				boneIndex = bone->index;
			}
			int slot = 0;
			while (slot < std::size(mVertices[vertexId].boneIndex) && (mVertices[vertexId].boneWeight[slot] > 0.000001)) {
				slot++;
			}
			if (slot < std::size(mVertices[vertexId].boneIndex)) {
				mVertices[vertexId].boneIndex[slot] = boneIndex;
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
	mMaterialList.resize(scene->mNumMaterials);
	for (int i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		mMaterialList[i] = std::make_shared<QMaterial>();
		int diffuseCount = material->GetTextureCount(aiTextureType_DIFFUSE);
		for (int j = 0; j < diffuseCount; j++) {
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, j, &path);
			mMaterialList[i]->addTextureSampler("Diffuse", QImage(QFileInfo(filePath).dir().filePath(path.C_Str())));
		}
		if (diffuseCount) {
			mMaterialList[i]->setShadingCode("FragColor = texture(Diffuse,vUV); ");
		}
		else {
			mMaterialList[i]->setShadingCode("FragColor = vec4(1.0); ");
		}
	}
	mSkeleton.reset(new QSkeleton(this, scene->mRootNode));
	mMeshes.clear();
	QQueue<QPair<aiNode*, aiMatrix4x4>> qNode;
	qNode.push_back({ scene->mRootNode ,aiMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<aiNode*, aiMatrix4x4> node = qNode.takeFirst();
		for (unsigned int i = 0; i < node.first->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node.first->mMeshes[i]];
			std::shared_ptr<QSkeletonMesh> skeletonMesh = std::make_shared<QSkeletonMesh>(this,mesh);
			skeletonMesh->setMaterial(mMaterialList[mesh->mMaterialIndex]);
			mMeshes << skeletonMesh;
		}
		for (unsigned int i = 0; i < node.first->mNumChildren; i++) {
			qNode.push_back({ node.first->mChildren[i] ,node.second * node.first->mChildren[i]->mTransformation });
		}
	}
}

std::shared_ptr<QSkeleton::BoneNode> QSkeletonModelComponent::getBoneNode(const QString& boneName)
{
	return mSkeleton->getBoneNode(boneName);
}
