#include "QRhiUniform.h"
#include "QRhiUniformMgr.h"

QRhiUniform::QRhiUniform()
	:mProxy(std::make_shared<QRhiUniformProxy>(this))
{
	QRhiUniformMgr::instance()->AddUniform(this);
}

QRhiUniform::~QRhiUniform()
{
	QRhiUniformMgr::instance()->RemoveUniform(this);
}

void QRhiUniform::addParamFloat(const QString& name, float var)
{
	addParam(name, static_cast<void*>(&var), sizeof(float), QParamDesc::Float);
}

void QRhiUniform::addParamVec2(const QString& name, QVector2D vec2)
{
	addParam(name, static_cast<void*>(&vec2), sizeof(QVector2D), QParamDesc::Vec2);
}

void QRhiUniform::addParamVec3(const QString& name, QVector3D vec3)
{
	addParam(name, static_cast<void*>(&vec3), sizeof(QVector3D), QParamDesc::Vec3);
}

void QRhiUniform::addParamVec4(const QString& name, QVector4D vec4)
{
	addParam(name, static_cast<void*>(&vec4), sizeof(QVector4D), QParamDesc::Vec4);
}

void QRhiUniform::addParamMat4(const QString& name, QMatrix4x4 mat4)
{
	addParam(name, static_cast<void*>(mat4.data()), sizeof(float) * 16, QParamDesc::Mat4);
}

void QRhiUniform::removeParam(const QString& name)
{
}

void QRhiUniform::setTextureSampler(const QString& name, const QImage& image)
{
	auto textureIter = getTextureDesc(name);
	if (textureIter != mTexture.end()) {
		textureIter->image = image;
		textureIter->needUpdate = true;
	}
}

void QRhiUniform::addTextureSampler(const QString& name, const QImage& image)
{
	QRhiUniform::QTextureDesc texture;
	texture.name = name;
	texture.image = image.convertToFormat(QImage::Format::Format_RGBA8888);
	auto it = image.format();
	texture.needUpdate = true;
	mTexture << texture;
	bNeedRecreate = true;
}

void QRhiUniform::removeTextureSampler(const QString& name)
{
}

void QRhiUniform::addParam(const QString& name, void* data, uint16_t size, QParamDesc::Type type)
{
	QParamDesc paramDesc;
	paramDesc.type = type;
	paramDesc.name = name;
	paramDesc.offsetInByte = mData.size();
	paramDesc.sizeInByte = size;
	paramDesc.sizeInByteAligned = (size + 15) & ~(15);	//十六字节对齐
	paramDesc.needUpdate = true;
	mData.resize(mData.size() + paramDesc.sizeInByteAligned);
	memcpy(mData.data() + paramDesc.offsetInByte, data, size);
	mParams << paramDesc;
	bNeedRecreate = true;
}

QVector<QRhiUniform::QParamDesc>::iterator QRhiUniform::getParamDesc(const QString& name)
{
	for (auto it = mParams.begin(); it != mParams.end(); ++it) {
		if (it->name == name) {
			return it;
		}
	}
	return mParams.end();
}

QVector<QRhiUniform::QTextureDesc>::iterator QRhiUniform::getTextureDesc(const QString& name)
{
	for (auto it = mTexture.begin(); it != mTexture.end(); ++it) {
		if (it->name == name) {
			return it;
		}
	}
	return mTexture.end();
}

QString QRhiUniform::QParamDesc::getTypeName()
{
	switch (type)
	{
	case QRhiUniform::QParamDesc::Float:
		return "float";
	case QRhiUniform::QParamDesc::Vec2:
		return "vec2";
	case QRhiUniform::QParamDesc::Vec3:
		return "vec3";
	case QRhiUniform::QParamDesc::Vec4:
		return "vec4";
	case QRhiUniform::QParamDesc::Mat4:
		return "mat4";
		break;
	default:
		break;
	}
	return "";
}