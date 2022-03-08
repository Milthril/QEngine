#include "QMaterial.h"

void QMaterial::addParamFloat(const QString& name, float var)
{
	addParam(name, static_cast<void*>(&var), sizeof(float), QMetaTypeId2<float>::qt_metatype_id());
}

void QMaterial::addParamVec2(const QString& name, QVector2D vec2)
{
	addParam(name, static_cast<void*>(&vec2), sizeof(QVector2D), QMetaTypeId2<QVector2D>::qt_metatype_id());
}

void QMaterial::addParamVec3(const QString& name, QVector3D vec3)
{
	addParam(name, static_cast<void*>(&vec3), sizeof(QVector3D), QMetaTypeId2<QVector3D>::qt_metatype_id());
}

void QMaterial::addParamVec4(const QString& name, QVector4D vec4)
{
	addParam(name, static_cast<void*>(&vec4), sizeof(QVector4D), QMetaTypeId2<QVector4D>::qt_metatype_id());
}

void QMaterial::addParamMat4(const QString& name, QMatrix4x4 mat4)
{
	addParam(name, static_cast<void*>(mat4.data()), sizeof(float) * 16, QMetaTypeId2<QMatrix4x4>::qt_metatype_id());
}

void QMaterial::removeParam(const QString& name)
{
}

void QMaterial::setTextureSampler(const QString& name, const QImage& image)
{
	auto textureIter = getTextureDesc(name);
	if (textureIter != mTexture.end()) {
		textureIter->image = image;
		textureIter->needUpdate = true;
	}
}

void QMaterial::addTextureSampler(const QString& name, const QImage& image)
{
	QMaterial::QTextureDesc texture;
	texture.name = name;
	texture.image = image;
	texture.needUpdate = true;
	mTexture << texture;
}

void QMaterial::removeTextureSampler(const QString& name)
{
}

void QMaterial::addParam(const QString& name, void* data, uint16_t size, int typeId)
{
	QParamDesc paramDesc;
	paramDesc.typeId = typeId;
	paramDesc.name = name;
	paramDesc.offsetInByte = mData.size();
	paramDesc.sizeInByte = size;
	paramDesc.sizeInByteAligned = (size + 15) & ~(15);	//Ê®Áù×Ö½Ú¶ÔÆë
	paramDesc.needUpdate = true;
	mData.resize(mData.size() + size);
	memcpy(mData.data() + paramDesc.offsetInByte, data, size);
}

QVector<QMaterial::QParamDesc>::iterator QMaterial::getParamDesc(const QString& name)
{
	for (auto it = mParams.begin(); it != mParams.end(); ++it) {
		if (it->name == name) {
			return it;
		}
	}
	return mParams.end();
}

QVector<QMaterial::QTextureDesc>::iterator QMaterial::getTextureDesc(const QString& name)
{
	for (auto it = mTexture.begin(); it != mTexture.end(); ++it) {
		if (it->name == name) {
			return it;
		}
	}
	return mTexture.end();
}