#include "QStaticModel.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include <QQueue>

QVector3D coveredAiVec3toQVec3(const aiVector3D& aiVec3) {
	return QVector3D(aiVec3.x, aiVec3.y, aiVec3.z);
}

std::shared_ptr<QStaticMeshComponent> createStaticMeshFromAssimpMesh(aiMesh* mesh, aiMatrix4x4 matrix) {
	std::shared_ptr<QStaticMeshComponent> staticMesh = std::make_shared<QStaticMeshComponent>();

	aiVector3D position;
	aiVector3D roatation;
	aiVector3D scale;
	matrix.Decompose(scale, roatation, position);

	staticMesh->setPosition(coveredAiVec3toQVec3(position));
	staticMesh->setRotation(coveredAiVec3toQVec3(roatation));
	staticMesh->setScale(coveredAiVec3toQVec3(scale));

	QVector<QStaticMeshComponent::Vertex> vertices(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		QStaticMeshComponent::Vertex& vertex = vertices[i];
		vertex.position = coveredAiVec3toQVec3(mesh->mVertices[i]);
		if (mesh->mNormals)
			vertex.normal = coveredAiVec3toQVec3(mesh->mNormals[i]);
		if (mesh->mTextureCoords[0]) {
			vertex.texCoord.setX(mesh->mTextureCoords[0][i].x);
			vertex.texCoord.setY(mesh->mTextureCoords[0][i].y);
		}
		if (mesh->mTangents)
			vertex.tangent = coveredAiVec3toQVec3(mesh->mTangents[i]);
		if (mesh->mBitangents)
			vertex.bitangent = coveredAiVec3toQVec3(mesh->mBitangents[i]);
	}
	staticMesh->setVertices(vertices);
	QVector<QStaticMeshComponent::Index> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	staticMesh->setIndices(indices);
	return staticMesh;
}

void QStaticModel::loadFromFile(const QString filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.toUtf8().constData(), aiProcess_Triangulate | aiProcess_FlipUVs);
	QQueue<QPair<aiNode*, aiMatrix4x4>> qNode;
	qNode.push_back({ scene->mRootNode ,aiMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<aiNode*, aiMatrix4x4> node = qNode.takeFirst();
		for (unsigned int i = 0; i < node.first->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node.first->mMeshes[i]];
			addChild(createStaticMeshFromAssimpMesh(mesh, node.second));
		}
		for (unsigned int i = 0; i < node.first->mNumChildren; i++) {
			qNode.push_back({ node.first->mChildren[i] ,node.second * node.first->mChildren[i]->mTransformation });
		}
	}
}