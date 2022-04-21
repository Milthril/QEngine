#include "QRhiUniform.h"
#include "Script\QLuaScriptFactory.h"
#include "ECS\System\RenderSystem\RHI\IRenderable.h"

QRhiUniform::QRhiUniform()
	: mProxy(std::make_shared<QRhiUniformProxy>(this))
	, mScript(QLuaScriptFactory::instance()->createUniformScript(this)) {
	bNeedRecreate.active();
	qDebug() << "create";
}

QRhiUniform::~QRhiUniform()
{
	qDebug() << "release";
}

void QRhiUniform::addDataFloat(const QString& name, float var)
{
	addDataParam(name, static_cast<void*>(&var), sizeof(float), ParamDescBase::Float);
}

void QRhiUniform::addDataVec2(const QString& name, QVector2D vec2)
{
	addDataParam(name, static_cast<void*>(&vec2), sizeof(QVector2D), ParamDescBase::Vec2);
}

void QRhiUniform::addDataVec3(const QString& name, QVector3D vec3)
{
	addDataParam(name, static_cast<void*>(&vec3), sizeof(QVector3D), ParamDescBase::Vec3);
}

void QRhiUniform::addDataVec4(const QString& name, QVector4D vec4)
{
	addDataParam(name, static_cast<void*>(&vec4), sizeof(QVector4D), ParamDescBase::Vec4);
}

void QRhiUniform::addDataMat4(const QString& name, QMatrix4x4 mat4)
{
	addDataParam(name, static_cast<void*>(mat4.data()), sizeof(float) * 16, ParamDescBase::Mat4);
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
				nextData->bNeedUpdate.active();
			}
		}
		for (int i = dataDesc->offsetInByte; i < mData.size() - dataDesc->sizeInByteAligned; i++) {
			mData[i] = mData[i + dataDesc->sizeInByteAligned];
		}
		mData.resize(mData.size() - dataDesc->sizeInByteAligned);
		mDataParamList.removeOne(std::dynamic_pointer_cast<DataDesc>(paramDesc));
	}
	else {
		mTextureParamList.removeOne(std::dynamic_pointer_cast<TextureDesc>(paramDesc));
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
		ptr->bNeedUpdate.active();
	}
}

void QRhiUniform::addTextureSampler(const QString& name, const QImage& image)
{
	std::shared_ptr<TextureDesc> texture = std::make_shared<TextureDesc>();
	texture->name = getVaildName(name);
	texture->type = QRhiUniform::TextureDesc::Sampler2D;
	texture->image = image.convertToFormat(QImage::Format::Format_RGBA8888);
	texture->bNeedUpdate.active();
	mParams << texture;
	mTextureParamList << texture;
	bNeedRecreate.active();
}

void QRhiUniform::addDataParam(const QString& name, void* data, uint16_t size, ParamDescBase::Type type)
{
	std::shared_ptr<DataDesc> paramDesc = std::make_shared<DataDesc>();
	paramDesc->type = type;
	paramDesc->name = getVaildName(name);
	paramDesc->offsetInByte = mData.size();
	paramDesc->sizeInByte = size;
	paramDesc->sizeInByteAligned = (size + 15) & ~(15);	//十六字节对齐
	paramDesc->bNeedUpdate.active();
	mData.resize(mData.size() + paramDesc->sizeInByteAligned);
	memcpy(mData.data() + paramDesc->offsetInByte, data, size);
	mParams << paramDesc;
	mDataParamList << paramDesc;
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


const QVector<std::shared_ptr<QRhiUniform::DataDesc>>& QRhiUniform::getAllDataDesc() {
	return mDataParamList;
}

const QVector<std::shared_ptr<QRhiUniform::TextureDesc>>& QRhiUniform::getAllTextureDesc() {
	return mTextureParamList;
}

void QRhiUniform::addRef(IRenderable* comp)
{
	mRef << comp;
}

void QRhiUniform::removeRef(IRenderable* comp)
{
	mRef.removeOne(comp);
}

void QRhiUniform::serialize(QDataStream& out) {
	out << mDataParamList.size();
	for (auto& data : mDataParamList) {
		out << data->name
			<< data->type
			<< data->offsetInByte
			<< data->sizeInByte
			<< data->sizeInByteAligned;
	}
	out << mTextureParamList.size();
	for (auto& texture : mTextureParamList) {
		out << texture->name
			<< texture->type
			<< texture->image;
	}
	out << mData;
}

void QRhiUniform::deserialize(QDataStream& in) {
	mDataParamList.clear();
	mTextureParamList.clear();
	mParams.clear();

	qsizetype numOfData = 0;
	in >> numOfData;
	for (int i = 0; i < numOfData; i++) {
		std::shared_ptr<DataDesc> data = std::make_shared<DataDesc>();
		in >> data->name
			>> data->type
			>> data->offsetInByte
			>> data->sizeInByte
			>> data->sizeInByteAligned;

		mDataParamList << data;
		mParams << data;
	}

	qsizetype numOfTexture = 0;
	in >> numOfTexture;
	for (int i = 0; i < numOfTexture; i++) {
		std::shared_ptr<TextureDesc> texture = std::make_shared<TextureDesc>();
		in >> texture->name
			>> texture->type
			>> texture->image;
		texture->image = texture->image.convertToFormat(QImage::Format::Format_RGBA8888);
		mTextureParamList << texture;
		mParams << texture;
	}
	in >> mData;
	bNeedRecreate.active();
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