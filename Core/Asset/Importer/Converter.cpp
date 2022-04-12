#include "Converter.h"

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
	staticMesh->setVertices(vertices);
	QVector<Asset::StaticMesh::Index> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	staticMesh->setIndices(indices);
	return staticMesh;
}


std::shared_ptr<Asset::Material> createMaterialFromAssimpMaterial(aiMaterial* material, const aiScene* scene, QDir dir) {
	auto qMaterial = std::make_shared<Asset::Material>();
	int diffuseCount = material->GetTextureCount(aiTextureType_DIFFUSE);
	for (int j = 0; j < diffuseCount; j++) {
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, j, &path);
		QString realPath = dir.filePath(path.C_Str());
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
		qMaterial->addTextureSampler("Diffuse", image);
	}
	if (diffuseCount) {
		qMaterial->setShadingCode("FragColor = texture(Diffuse,vUV); ");
	}
	else {
		qMaterial->setShadingCode("FragColor = vec4(1.0); ");
	}
	return qMaterial;
}

