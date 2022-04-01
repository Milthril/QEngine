#include "QRhiUniform.h"
#include "Script\QLuaScriptFactory.h"

QRhiUniform::QRhiUniform()
	: mProxy(std::make_shared<QRhiUniformProxy>(this))
	, mScript(QLuaScriptFactory::instance()->createUniformScript(this)) {
	bNeedRecreate.active();
}

QRhiUniform::~QRhiUniform()
{
}

void QRhiUniform::addDataFloat(const QString& name, float var)
{
	addParam(name, static_cast<void*>(&var), sizeof(float), ParamDescBase::Float);
}

void QRhiUniform::addDataVec2(const QString& name, QVector2D vec2)
{
	addParam(name, static_cast<void*>(&vec2), sizeof(QVector2D), ParamDescBase::Vec2);
}

void QRhiUniform::addDataVec3(const QString& name, QVector3D vec3)
{
	addParam(name, static_cast<void*>(&vec3), sizeof(QVector3D), ParamDescBase::Vec3);
}

void QRhiUniform::addDataVec4(const QString& name, QVector4D vec4)
{
	addParam(name, static_cast<void*>(&vec4), sizeof(QVector4D), ParamDescBase::Vec4);
}

void QRhiUniform::addDataMat4(const QString& name, QMatrix4x4 mat4)
{
	addParam(name, static_cast<void*>(mat4.data()), sizeof(float) * 16, ParamDescBase::Mat4);
}

void QRhiUniform::removeParam(const QString& name)
{
	std::shared_ptr<ParamDescBase> paramDesc = getParamDesc(name);
	if (!paramDesc)
		return;
	std::shared_ptr<DataDesc> dataDesc = std::dynamic_pointer_cast<DataDesc>(paramDesc);
	if (dataDesc) {
		int index = mParams.indexOf(paramDesc);
		for (int i = index + 1; i < mParams.size(); i++) {
			std::shared_ptr<DataDesc> nextData = std::dynamic_pointer_cast<DataDesc>(mParams[i]);
			if (nextData) {
				nextData->offsetInByte -= dataDesc->sizeInByteAligned;
				nextData->needUpdate.active();
			}
		}
		for (int i = dataDesc->offsetInByte; i < mData.size() - dataDesc->sizeInByteAligned; i++) {
			mData[i] = mData[i + dataDesc->sizeInByteAligned];
		}
		mData.resize(mData.size() - dataDesc->sizeInByteAligned);
	}
	mParams.removeOne(paramDesc);
	bNeedRecreate.active();
}

bool QRhiUniform::renameParma(const QString& src, const QString& dst)
{
	if (dst == src)
		return false;
	auto param = getParamDesc(src);
	if (param) {
		param->name = getVaildName(dst);
		bNeedRecreate.active();
		return true;
	}
	return false;
}

QString QRhiUniform::getVaildName(QString name)
{
	bool flag = true;
	while (flag) {
		flag = false;
		for (auto& param : mParams) {
			if (param->name == name) {
				flag = true;
				name += "1";
				break;;
			}
		}
	}
	return name;
}

void QRhiUniform::setTextureSampler(const QString& name, const QImage& image)
{
	auto texture = getParamDesc(name);
	if (texture && texture->type == QRhiUniform::ParamDescBase::Sampler2D) {
		std::shared_ptr<QRhiUniform::TextureDesc> ptr = std::dynamic_pointer_cast<QRhiUniform::TextureDesc>(texture);
		ptr->image = image;
		ptr->needUpdate.active();
	}
}

void QRhiUniform::addTextureSampler(const QString& name, const QImage& image)
{
	std::shared_ptr<TextureDesc> texture = std::make_shared<TextureDesc>();
	texture->name = getVaildName(name);
	texture->type = QRhiUniform::TextureDesc::Sampler2D;
	texture->image = image.convertToFormat(QImage::Format::Format_RGBA8888);
	auto it = image.format();
	texture->needUpdate.active();
	mParams << texture;
	bNeedRecreate.active();
}

void QRhiUniform::addParam(const QString& name, void* data, uint16_t size, ParamDescBase::Type type)
{
	std::shared_ptr<DataDesc> paramDesc = std::make_shared<DataDesc>();
	paramDesc->type = type;
	paramDesc->name = getVaildName(name);
	paramDesc->offsetInByte = mData.size();
	paramDesc->sizeInByte = size;
	paramDesc->sizeInByteAligned = (size + 15) & ~(15);	//十六字节对齐
	paramDesc->needUpdate.active();
	mData.resize(mData.size() + paramDesc->sizeInByteAligned);
	memcpy(mData.data() + paramDesc->offsetInByte, data, size);
	mParams << paramDesc;
	bNeedRecreate.active();
}

std::shared_ptr<QRhiUniform::ParamDescBase> QRhiUniform::getParamDesc(const QString& name)
{
	for (auto it = mParams.begin(); it != mParams.end(); ++it) {
		if ((*it)->name == name) {
			return *it;
		}
	}
	return nullptr;
}

QVector<std::shared_ptr<QRhiUniform::DataDesc>> QRhiUniform::getAllDataDesc()
{
	QVector<std::shared_ptr<QRhiUniform::DataDesc>> vec;
	for (auto param : mParams) {
		std::shared_ptr<QRhiUniform::DataDesc> data = std::dynamic_pointer_cast<DataDesc>(param);
		if (data)
			vec << data;
	}
	return vec;
}

QVector<std::shared_ptr<QRhiUniform::TextureDesc>> QRhiUniform::getAllTextureDesc()
{
	QVector<std::shared_ptr<QRhiUniform::TextureDesc>> vec;
	for (auto param : mParams) {
		std::shared_ptr<QRhiUniform::TextureDesc> tex = std::dynamic_pointer_cast<TextureDesc>(param);
		if (tex)
			vec << tex;
	}
	return vec;
}

void QRhiUniform::addRef(QSceneComponent* comp)
{
	mRef << comp;
}

void QRhiUniform::removeRef(QSceneComponent* comp)
{
	mRef.removeOne(comp);
}

QString QRhiUniform::ParamDescBase::getTypeName()
{
	switch (type)
	{
	case QRhiUniform::ParamDescBase::Float:
		return "float";
	case QRhiUniform::ParamDescBase::Vec2:
		return "vec2";
	case QRhiUniform::ParamDescBase::Vec3:
		return "vec3";
	case QRhiUniform::ParamDescBase::Vec4:
		return "vec4";
	case QRhiUniform::ParamDescBase::Mat4:
		return "mat4";
	case QRhiUniform::ParamDescBase::Sampler2D:
		return "sampler2D";
	default:
		break;
	}
	return "";
}