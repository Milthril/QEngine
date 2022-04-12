#include "ImporterTask.h"
#include "QRegularExpression"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include "QQueue"
#include "Converter.h"
#include "..\Material.h"
#include "..\StaticMesh.h"
#include "Serialization\QSerialization.h"


void ImporterTask::executable() {
	if (!QFile::exists(mFilePath))
		return;
	QFileInfo info(mFilePath);
	if (info.suffix().compare((".FBX"), Qt::CaseInsensitive)) {
		resolveModel();
	}
}

void ImporterTask::resolveModel() {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(mFilePath.toUtf8().constData(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene) {
		return;
	}
	QList<std::shared_ptr<Asset::Material>> mMaterialList;
	for (int i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		auto qMaterial = createMaterialFromAssimpMaterial(material, scene, QFileInfo(mFilePath).dir());
		mMaterialList << qMaterial;
		QString vaildPath = getVaildPath(mDestDir.filePath(QString("%1.QAsset").arg(material->GetName().C_Str())));
		qMaterial->setName(QFileInfo(vaildPath).baseName());
		QFile file(vaildPath);
		if (file.open(QFile::WriteOnly)) {
			QDataStream out(&file);
			out << mMaterialList.back().get();
			file.close();
		}
	}
	QQueue<QPair<aiNode*, aiMatrix4x4>> qNode;
	qNode.push_back({ scene->mRootNode ,aiMatrix4x4() });
	while (!qNode.isEmpty()) {
		QPair<aiNode*, aiMatrix4x4> node = qNode.takeFirst();
		for (unsigned int i = 0; i < node.first->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node.first->mMeshes[i]];
			std::shared_ptr<Asset::StaticMesh> staticMesh = createStaticMeshFromAssimpMesh(mesh);
			staticMesh->setMaterial(*mMaterialList[mesh->mMaterialIndex]);
			QString vaildPath = getVaildPath(mDestDir.filePath(QString("%1.QAsset").arg(mesh->mName.C_Str())));
			staticMesh->setName(QFileInfo(vaildPath).baseName());
			QFile file(vaildPath);
			if (file.open(QFile::WriteOnly)) {
				QDataStream out(&file);
				out << staticMesh.get();
			}
		}
		for (unsigned int i = 0; i < node.first->mNumChildren; i++) {
			qNode.push_back({ node.first->mChildren[i] ,node.second * node.first->mChildren[i]->mTransformation });
		}
	}
}

QString ImporterTask::getVaildPath(QString path) {
	//if (!QFile::exists(path))
		return path;
	//QFileInfo info(path);
	//QString baseName = info.baseName();
	//QString newPath;
	//do {
	//	baseName += "_1";
	//	newPath = info.dir().filePath(baseName +"." + info.suffix());
	//} while (QFile::exists(newPath));
	//return newPath;
}

