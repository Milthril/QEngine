#include "IAsset.h"
#include "QDataStream"
#include "Serialization/QSerialization.h"
#include "Utils/FileUtils.h"
#include "Utils/PicoSHA2.h"
#include "GAssetMgr.h"
#include "QVariant"

QString IAsset::getName() const {
	return QFileInfo(mRefPath).baseName();
}

QString IAsset::getRefPath() const {
	return mRefPath;
}

void IAsset::setRefPath(QString val) {
	mRefPath = val;
}

void IAsset::save(QString filePath, bool cover /*= true*/) {
	QByteArray data;
	QDataStream stream(&data, QFile::WriteOnly);
	stream << (int)this->type();
	serialize(stream);

	//std::string sha256;
	//picosha2::hash256_hex_string(data, sha256);
	//TheAssetMgr->updateSHA256(filePath, QByteArray::fromStdString(sha256));

	QFile file(filePath);
	if (file.open(QFile::WriteOnly)) {
		file.write(data);
		file.close();
	}
}

QByteArray IAsset::getSHA() {
	return TheAssetMgr->getSHA256(mRefPath);
}

QString IAsset::getExtName() {
	return AssetExtName[type()];
}
