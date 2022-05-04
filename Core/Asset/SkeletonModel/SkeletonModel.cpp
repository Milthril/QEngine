#include "SkeletonModel.h"
#include "Serialization/QSerialization.h"

namespace Asset{

QDataStream& operator<<(QDataStream& out, const Asset::SkeletonModel::Vertex& vertex) {
	out << vertex.position;
	out << vertex.normal;
	out << vertex.tangent;
	out << vertex.bitangent;
	out << vertex.texCoord;
	out << vertex.boneIndex[0] << vertex.boneIndex[1] << vertex.boneIndex[2] << vertex.boneIndex[3];
	out << vertex.boneWeight;
	return out;
}

QDataStream& operator<<(QDataStream& out, const Asset::SkeletonModel::Mesh& mesh) {
	out << mesh.mVerticesOffset;
	out << mesh.mVerticesRange;
	out << mesh.mIndicesOffset;
	out << mesh.mIndicesRange;
	out << mesh.mMaterialIndex;
	return out;
}

QDataStream& operator>>(QDataStream& in, Asset::SkeletonModel::Vertex& vertex) {
	in >> vertex.position;
	in >> vertex.normal;
	in >> vertex.tangent;
	in >> vertex.bitangent;
	in >> vertex.texCoord;
	in >> vertex.boneIndex[0] >> vertex.boneIndex[1] >> vertex.boneIndex[2] >> vertex.boneIndex[3];
	in >> vertex.boneWeight;
	return in;
}

QDataStream& operator>>(QDataStream& in, Asset::SkeletonModel::Mesh& mesh) {
	in >> mesh.mVerticesOffset;
	in >> mesh.mVerticesRange;
	in >> mesh.mIndicesOffset;
	in >> mesh.mIndicesRange;
	in >> mesh.mMaterialIndex;
	return in;
}

void SkeletonModel::deserialize(QDataStream& in) {
	IAsset::deserialize(in);
	in >> mMeshList;
	in >> mMaterialPathList;
	in >> mSkeletonPath;
	in >> mVertices;
	in >> mIndices;
}

void SkeletonModel::serialize(QDataStream& out) {
	IAsset::serialize(out);
	out << mMeshList;
	out << mMaterialPathList;
	out << mSkeletonPath;
	out << mVertices;
	out << mIndices;
}

}


