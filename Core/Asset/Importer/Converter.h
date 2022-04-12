#ifndef Converter_h__
#define Converter_h__


#include "qvectornd.h"
#include "assimp\vector3.h"
#include "assimp\matrix4x4.h"
#include "assimp\mesh.h"
#include "assimp\material.h"
#include "assimp\scene.h"
#include "QMatrix4x4"
#include "QDir"

#include "Asset\StaticMesh.h"
#include "Asset\Material.h"


QVector3D converter(const aiVector3D& aiVec3);

QMatrix4x4 converter(const aiMatrix4x4& aiMat4);

std::shared_ptr<Asset::StaticMesh> createStaticMeshFromAssimpMesh(aiMesh* mesh);

std::shared_ptr<Asset::Material> createMaterialFromAssimpMaterial(aiMaterial* material,const aiScene* scene, QDir dir);

#endif // Converter_h__
