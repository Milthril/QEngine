#include "ImporterModelTask.h"
#include "Asset\Material.h"
#include "Asset\StaticMesh.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include "QQueue"
#include "Utils\FileUtils.h"
#include "Asset\SkeletonModel\SkeletonModel.h"
#include "Asset\SkeletonModel\Skeleton.h"
#include "Asset\GAssetMgr.h"
#include "Asset\SkeletonModel\SkeletonAnimation.h"

QMatrix4x4 converter(const aiMatrix4x4& aiMat4) {
	QMatrix4x4 mat4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat4(i, j) = aiMat4[i][j];
		}
	}
	return mat4;
}

QVector3D converter(const aiVector3D& aiVec3) {
	return QVector3D(aiVec3.x, aiVec3.y, aiVec3.z);
}

std::shared_ptr<Asset::StaticMesh> createStaticMeshFromAssimpMesh(aiMesh* mesh) {
	std::shared_ptr<Asset::StaticMesh> staticMesh = std::make_shared<Asset::StaticMesh>();
	QVector<Asset::StaticMesh::Vertex> vertices(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Asset::StaticMesh::Vertex& vertex = vertices[i];
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
	staticMesh->setVertices(std::move(vertices));
	QVector<Asset::StaticMesh::Index> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	staticMesh->setIndices(std::move(indices));
	return staticMesh;
}

std::shared_ptr<Asset::Material> createMaterialFromAssimpMaterial(aiMaterial* material, const aiScene* scene, QDir dir) {
	auto qMaterial = std::make_shared<Asset::Material>();
	for (int i = aiTextureType_DIFFUSE ; i < AI_TEXTURE_TYPE_MAX; i++) {
		int count = material->GetTextureCount(aiTextureType(i));
		for (int j = 0; j < count; j++) {
			aiString path;
			material->GetTexture(aiTextureType(i), j, &path);
			QString name = material->GetName().C_Str();
			if (name.startsWith('/')) {
				QString newPath = dir.filePath(name.mid(1, name.lastIndexOf('/') - 1));
				dir.setPath(newPath);
			}
			QString realPath = dir.filePath(path.C_Str());

			QImage image;
			if (QFile::exists(realPath)) {
				image.load(realPath);
			}
			else {
				const aiTexture* embTexture = scene->GetEmbeddedTexture(path.C_Str());
				if (embTexture != nullptr) {
					if (embTexture->mHeight == 0) {
						image.loadFromData((uchar*)embTexture->pcData, embTexture->mWidth, embTexture->achFormatHint);
					}
					else {
						image = QImage((uchar*)embTexture->pcData, embTexture->mWidth, embTexture->mHeight, QImage::Format_ARGB32);
					}
				}
			}
			QString textureName = ImporterModelTask::mTextureNameMap[i];
			if (j != 0) {
				textureName += QString::number(j);
			}
			qMaterial->addTextureSampler(textureName, image);
		}
	}
	int hasDiffuseTexture = material->GetTextureCount(aiTextureType_DIFFUSE);
	if (hasDiffuseTexture > 0) {
		qMaterial->setShadingCode("FragColor = texture(Diffuse,vUV); ");
	}
	else {
		qMaterial->setShadingCode("FragColor = vec4(1.0); ");
	}
	return qMaterial;
}

std::shared_ptr<Asset::Skeleton::ModelNode> processSkeletonModelNode(aiNode* node) {
	std::shared_ptr<Asset::Skeleton::ModelNode> boneNode = std::make_shared<Asset::Skeleton::ModelNode>();
	boneNode->name = node->mName.C_Str();
	boneNode->localMatrix = converter(node->mTransformation);
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		boneNode->children.push_back(processSkeletonModelNode(node->mChildren[i]));
	}
	return boneNode;
}

void ImporterModelTask::executable() {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(mFilePath.toUtf8().constData(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene) {
		return;
	}

	QStringList mMaterialPathList;
	for (uint i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		auto qMaterial = createMaterialFromAssimpMaterial(material, scene, QFileInfo(mFilePath).dir());
		QString name = material->GetName().C_Str();
		QString vaildPath = FileUtils::getVaildPath(mDestDir.filePath(QString("%1.%2").arg(name.split('/').back()).arg(qMaterial->getExtName())));;
		qMaterial->setRefPath(vaildPath);
		qMaterial->save(vaildPath);
		mMaterialPathList << qMaterial->getRefPath();
	}

	bool hasBone = false;

	QQueue<QPair<aiNode*, aiMatrix4x4>> qNode;
	qNode.push_back({ scene->mRootNode ,aiMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<aiNode*, aiMatrix4x4> node = qNode.takeFirst();
		for (unsigned int i = 0; i < node.first->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node.first->mMeshes[i]];
			std::shared_ptr<Asset::StaticMesh> staticMesh = createStaticMeshFromAssimpMesh(mesh);
			staticMesh->setMaterialPath(mMaterialPathList[mesh->mMaterialIndex]);
			QString vaildPath = FileUtils::getVaildPath(mDestDir.filePath(QString("%1.%2").arg(mesh->mName.C_Str()).arg(staticMesh->getExtName())));
			staticMesh->setRefPath(vaildPath);
			staticMesh->save(vaildPath);

			if (mesh->HasBones()) {
				hasBone = true;
			}
		}
		for (unsigned int i = 0; i < node.first->mNumChildren; i++) {
			qNode.push_back({ node.first->mChildren[i] ,node.second * node.first->mChildren[i]->mTransformation });
		}
	}

	if (hasBone) {
		QVector<Asset::SkeletonModel::Mesh> mMeshList;
		std::shared_ptr<Asset::Skeleton::ModelNode> mRootNode = processSkeletonModelNode(scene->mRootNode);
		QHash<QString, std::shared_ptr<Asset::Skeleton::BoneNode>> mBoneMap;
		QVector<Asset::Skeleton::Mat4> mBoneOffsetMatrix;
		QVector<Asset::Skeleton::Mat4> mCurrentPosesMatrix;

		qNode.push_back({ scene->mRootNode ,aiMatrix4x4() });
		while (!qNode.isEmpty()) {
			QPair<aiNode*, aiMatrix4x4> node = qNode.takeFirst();
			for (unsigned int i = 0; i < node.first->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node.first->mMeshes[i]];
				Asset::SkeletonModel::Mesh mMesh;
				mMesh.mVertices.resize(mesh->mNumVertices);
				for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
					Asset::SkeletonModel::Vertex& vertex = mMesh.mVertices[i];
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

				for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
					aiFace face = mesh->mFaces[i];
					for (unsigned int j = 0; j < face.mNumIndices; j++) {
						mMesh.mIndices.push_back(face.mIndices[j]);
					}
				}

				for (unsigned int i = 0; i < mesh->mNumBones; i++) {
					for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
						int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
						aiBone* bone = mesh->mBones[i];
						std::shared_ptr<Asset::Skeleton::BoneNode> boneNode;
						if (mBoneMap.contains(bone->mName.C_Str()))
							boneNode = mBoneMap[bone->mName.C_Str()];
						else {
							boneNode = std::make_shared<Asset::Skeleton::BoneNode>();
							boneNode->name = bone->mName.C_Str();
							boneNode->offsetMatrix = converter(bone->mOffsetMatrix);
							boneNode->index = mBoneOffsetMatrix.size();
							mBoneOffsetMatrix << boneNode->offsetMatrix.toGenericMatrix<4, 4>();
							mBoneMap[boneNode->name] = boneNode;
						}
						int slot = 0;
						while (slot < std::size(mMesh.mVertices[vertexId].boneIndex) && (mMesh.mVertices[vertexId].boneWeight[slot] > 0.000001)) {
							slot++;
						}
						if (slot < std::size(mMesh.mVertices[vertexId].boneIndex)) {
							mMesh.mVertices[vertexId].boneIndex[slot] = boneNode->index;
							mMesh.mVertices[vertexId].boneWeight[slot] = mesh->mBones[i]->mWeights[j].mWeight;;
						}
						else {
							qWarning("Lack of slot");
						}
					}
				}
				mMesh.mMaterialIndex = mesh->mMaterialIndex;
				mMeshList.emplace_back(std::move(mMesh));
			}
			for (unsigned int i = 0; i < node.first->mNumChildren; i++) {
				qNode.push_back({ node.first->mChildren[i] ,node.second * node.first->mChildren[i]->mTransformation });
			}
		}

		std::shared_ptr<Asset::Skeleton> mSkeleton = std::make_shared<Asset::Skeleton>();
		mSkeleton->setBoneMap(std::move(mBoneMap));
		mSkeleton->setBoneOffsetMatrix(std::move(mBoneOffsetMatrix));
		mSkeleton->setRootNode(mRootNode);
		mSkeleton->setRefPath(mDestDir.filePath("%1.%2").arg(QFileInfo(mFilePath).baseName()).arg(mSkeleton->getExtName()));
		mSkeleton->save(mSkeleton->getRefPath());

		std::shared_ptr<Asset::SkeletonModel> mSkeletonModel = std::make_shared<Asset::SkeletonModel>();
		mSkeletonModel->setMeshList(std::move(mMeshList));
		mSkeletonModel->setSkeletonPath(mSkeleton->getRefPath());
		mSkeletonModel->setRefPath(mDestDir.filePath("%1.%2").arg(QFileInfo(mFilePath).baseName()).arg(mSkeletonModel->getExtName()));
		mSkeletonModel->setMaterialPathList(std::move(mMaterialPathList));
		mSkeletonModel->save(mSkeletonModel->getRefPath());

	}

	for (uint i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation* anim = scene->mAnimations[i];
		std::shared_ptr<Asset::SkeletonAnimation> mSkeletonAnim = std::make_shared<Asset::SkeletonAnimation>();
		mSkeletonAnim->mDuration = anim->mDuration;
		mSkeletonAnim->mTicksPerSecond = anim->mTicksPerSecond;
		for (unsigned int j = 0; j < anim->mNumChannels; j++) {
			aiNodeAnim* node = anim->mChannels[j];
			Asset::SkeletonAnimation::Node& skNode = mSkeletonAnim->mAnimNode[node->mNodeName.C_Str()];
			for (uint j = 0; j < node->mNumScalingKeys; j++) {
				const aiVectorKey& key = node->mScalingKeys[j];
				skNode.scaling[key.mTime/ mSkeletonAnim->mTicksPerSecond] = QVector3D(key.mValue.x, key.mValue.y, key.mValue.z);
			}
			for (uint j = 0; j < node->mNumPositionKeys; j++) {
				const aiVectorKey& key = node->mPositionKeys[j];
				skNode.translation[key.mTime/ mSkeletonAnim->mTicksPerSecond] = QVector3D(key.mValue.x, key.mValue.y, key.mValue.z);
			}
			for (uint j = 0; j < node->mNumRotationKeys; j++) {
				const aiQuatKey& key = node->mRotationKeys[j];
				skNode.rotation[key.mTime/ mSkeletonAnim->mTicksPerSecond] = QQuaternion(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
			}
		}
		QString name = anim->mName.C_Str();
		if (name.isEmpty())
			name = QFileInfo(mFilePath).baseName() + "_Animation";
		mSkeletonAnim->setRefPath(mDestDir.filePath("%1.%2").arg(name).arg(mSkeletonAnim->getExtName()));
		//mSkeletonAnim->mSkeletonSHA = mSkeleton->getSHA();
		mSkeletonAnim->save(mSkeletonAnim->getRefPath());
	}
}

