#include "ImporterTask.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include "Converter.h"
#include "QQueue"
#include "QRegularExpression"
#include "Serialization\QSerialization.h"

#include "Asset\SkyBox.h"
#include "Asset\Material.h"
#include "Asset\StaticMesh.h"


void ImporterTask::executable() {
	if (!QFile::exists(mFilePath))
		return;
	QFileInfo info(mFilePath);
	QString suffix = info.suffix();
	if (suffix.compare(("FBX"), Qt::CaseInsensitive)==0
		|| suffix.compare(("MMD"), Qt::CaseInsensitive) == 0
		|| suffix.compare(("OBJ"), Qt::CaseInsensitive) == 0
		) {
		resolveModel();
	}
	else if (suffix.compare(("PNG"), Qt::CaseInsensitive)==0
		|| suffix.compare(("JPG"), Qt::CaseInsensitive)==0
		|| suffix.compare(("JPEG"), Qt::CaseInsensitive)==0
		|| suffix.compare(("TGA"), Qt::CaseInsensitive)==0
			 ) {
		resolveImage();
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
		QString vaildPath = getVaildPath(mDestDir.filePath(QString("%1.%2").arg(material->GetName().C_Str()).arg(qMaterial->getExtName())));;
		qMaterial->setRefPath(vaildPath);
		QFile file(vaildPath);
		if (file.open(QFile::WriteOnly)) {
			QDataStream out(&file);
			out << qMaterial.get();
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
			staticMesh->setMaterialPath(mMaterialList[mesh->mMaterialIndex]->getRefPath());
			QString vaildPath = getVaildPath(mDestDir.filePath(QString("%1.%2").arg(mesh->mName.C_Str()).arg(staticMesh->getExtName())));
			staticMesh->setRefPath(vaildPath);
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

void ImporterTask::resolveImage() {
	
	QImage image(mFilePath);
	if (image.isNull())
		return;
	image = image.convertToFormat(QImage::Format::Format_RGBA8888);
	QSize mCubeFaceSize;
	if (image.width() == image.height())
		mCubeFaceSize = image.size();
	else if (image.width() * 3 == 4 * image.height())
		mCubeFaceSize = QSize(image.width() / 4, image.width() / 4);
	else if (image.width() * 4 == 3 * image.height())
		mCubeFaceSize = QSize(image.width() / 3, image.width() / 3);
	else
		return;
	std::shared_ptr<Asset::SkyBox> skyBox = std::make_shared<Asset::SkyBox>();
	std::array<QImage, 6> imageList;
	// 依次为左右上下前后
	imageList[0] = image.copy(QRect(QPoint(2 * mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));
	imageList[1] = image.copy(QRect(QPoint(0, mCubeFaceSize.width()), mCubeFaceSize));

	imageList[2] = image.copy(QRect(QPoint(mCubeFaceSize.width(), 0), mCubeFaceSize));
	imageList[3] = image.copy(QRect(QPoint(mCubeFaceSize.width(), mCubeFaceSize.width() * 2), mCubeFaceSize));

	imageList[4] = image.copy(QRect(QPoint(mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));
	imageList[5] = image.copy(QRect(QPoint(3 * mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));

	skyBox->setImageList(imageList);
	QString vaildPath = getVaildPath(mDestDir.filePath(QString("%1.%2").arg(QFileInfo(mFilePath).baseName()).arg(skyBox->getExtName())));
	skyBox->setRefPath(vaildPath);
	QFile file(vaildPath);
	if (file.open(QFile::WriteOnly)) {
		QDataStream out(&file);
		out << skyBox.get();
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

