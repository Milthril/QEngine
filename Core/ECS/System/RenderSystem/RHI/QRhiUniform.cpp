#include "QRhiUniform.h"
#include "Script\QLuaScriptFactory.h"
#include "ECS\System\RenderSystem\RHI\IRenderable.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"

QRhiUniform::QRhiUniform()
	: mScript(QLuaScriptFactory::instance()->createUniformScript(this)) {
	bNeedRecreate.active();
}

void QRhiUniform::setDataFloat(QString name, float var) {
	auto param = getOrCreateParam(name, ParamDescBase::Float);
	auto data = std::dynamic_pointer_cast<DataDesc>(param);
	data->var = var;
	data->bNeedUpdate.active();
}

void QRhiUniform::setDataVec2(QString name, QVector2D var) {
	auto param = getOrCreateParam(name, ParamDescBase::Vec2);
	auto data = std::dynamic_pointer_cast<DataDesc>(param);
	data->var = var;
	data->bNeedUpdate.active();
}

void QRhiUniform::setDataVec3(QString name, QVector3D var) {
	auto param = getOrCreateParam(name, ParamDescBase::Vec3);
	auto data = std::dynamic_pointer_cast<DataDesc>(param);
	data->var = var;
	data->bNeedUpdate.active();
}

void QRhiUniform::setDataVec4(QString name, QVector4D var) {
	auto param = getOrCreateParam(name, ParamDescBase::Vec4);
	auto data = std::dynamic_pointer_cast<DataDesc>(param);
	data->var = var;
	data->bNeedUpdate.active();
}

void QRhiUniform::setDataColor4(QString name, QColor4D var) {
	auto param = getOrCreateParam(name, ParamDescBase::Color4);
	auto data = std::dynamic_pointer_cast<DataDesc>(param);
	data->var = var;
	data->bNeedUpdate.active();
}

void QRhiUniform::setTexture2D(const QString& name, const QImage& image) {
	auto param = getOrCreateParam(name, ParamDescBase::Sampler2D);
	auto texture = std::dynamic_pointer_cast<TextureDesc>(param);
	texture->image = image;
	texture->bNeedUpdate.active();
}

void QRhiUniform::removeParam(const QString& name)
{
	auto iter = mParamNameMap.find(name);
	if (iter != mParamNameMap.end()) {
		mTextureList.removeOne(*iter);
		mDataList.removeOne(*iter);
		mParamNameMap.remove(name);
		updateLayout();
	}
}

bool QRhiUniform::renameParma(const QString& src, const QString& dst)
{
	if (mParamNameMap.contains(src)) {
		auto param = mParamNameMap.take(src);
		param->name = dst;
		mParamNameMap[dst] = param;
		updateLayout();
		return true;
	}
	return false;
}

void QRhiUniform::addRef(IRenderable* comp)
{
	mRef << comp;
}

void QRhiUniform::removeRef(IRenderable* comp){
	mRef.removeOne(comp);
}

void QRhiUniform::serialize(QDataStream& out) {
	out << mDataList.size();
	for (auto& data : mDataList) {
		out << data->name
			<< data->var
			<< data->type
			<< data->offsetInByte
			<< data->sizeInByte
			<< data->sizeInByteAligned;
	}
	out << mTextureList.size();
	for (auto& texture : mTextureList) {
		out << texture->name
			<< texture->type
			<< texture->image;
	}
}

void QRhiUniform::deserialize(QDataStream& in) {
	mDataList.clear();
	mTextureList.clear();
	mParamNameMap.clear();
	qsizetype numOfData = 0;
	in >> numOfData;
	for (int i = 0; i < numOfData; i++) {
		std::shared_ptr<DataDesc> data = std::make_shared<DataDesc>();
		in >> data->name
			>> data->var
			>> data->type
			>> data->offsetInByte
			>> data->sizeInByte
			>> data->sizeInByteAligned;
		mDataList << data;
		mParamNameMap[data->name] = data;
	}

	qsizetype numOfTexture = 0;
	in >> numOfTexture;
	for (int i = 0; i < numOfTexture; i++) {
		std::shared_ptr<TextureDesc> texture = std::make_shared<TextureDesc>();
		in >> texture->name
			>> texture->type
			>> texture->image;
		texture->image = texture->image.convertToFormat(QImage::Format::Format_RGBA8888);
		mTextureList << texture;
		mParamNameMap[texture->name] = texture;
	}
	updateLayout();
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
	case QRhiUniform::ParamDescBase::Color4:
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

std::shared_ptr<QRhiUniform::ParamDescBase> QRhiUniform::getOrCreateParam(QString name, ParamDescBase::Type type) {
	auto Iter = mParamNameMap.find(name);
	if (Iter != mParamNameMap.end()) {
		return *Iter;
	}
	else {
		std::shared_ptr<QRhiUniform::ParamDescBase> param;
		if (type != ParamDescBase::Type::Sampler2D) {
			mDataList << std::make_shared<QRhiUniform::DataDesc>();
			param = mDataList.back();
		}
		else {
			mTextureList << std::make_shared<QRhiUniform::TextureDesc>();;
			param = mTextureList.back();
		}
		param->type = type;
		param->name = getVaildName(name);
		mParamNameMap[param->name] = param;
		updateLayout();
		return param;
	}
}

QString QRhiUniform::getVaildName(QString name) {
	QString newName = name;
	int index = 0;
	while (mParamNameMap.contains(newName)) {
		newName = name + QString::number(index);
	}
	return newName;
}

int getByteSize(QRhiUniform::ParamDescBase::Type type) {
	switch (type) {
	case QRhiUniform::ParamDescBase::Float:
		return sizeof(float);
	case QRhiUniform::ParamDescBase::Vec2:
		return sizeof(float) * 2;
	case QRhiUniform::ParamDescBase::Vec3:
		return sizeof(float) * 3;
	case QRhiUniform::ParamDescBase::Vec4:
	case QRhiUniform::ParamDescBase::Color4:
		return sizeof(float) * 4;
	case QRhiUniform::ParamDescBase::Mat4:
		return sizeof(float) * 16;
	}
	return 0;
}

int getAlign(QRhiUniform::ParamDescBase::Type type) {
	switch (type) {
	case QRhiUniform::ParamDescBase::Float:
		return sizeof(float);
	case QRhiUniform::ParamDescBase::Vec2:
		return sizeof(float) * 2;
	case QRhiUniform::ParamDescBase::Vec3:
		return sizeof(float) * 4;
	case QRhiUniform::ParamDescBase::Vec4:
		return sizeof(float) * 4;
	case QRhiUniform::ParamDescBase::Mat4:
		return sizeof(float) * 16;
	}
	return 0;
}

int align(int size, int alignSize) {
	if (size == alignSize)
		return size;
	return (size + alignSize - 1) & ~(alignSize - 1);
}

void QRhiUniform::updateLayout() {
	mDataSize = 0;
	for (int i = 0; i < mDataList.size(); i++) {
		auto& data = mDataList[i];
		data->offsetInByte = align(mDataSize,getAlign(data->type));
		data->sizeInByte = getByteSize(data->type);
		data->sizeInByteAligned = data->sizeInByte;	//16字节对齐
		data->bNeedUpdate.active();
		mDataSize = data->offsetInByte+data->sizeInByteAligned;
	}
	bNeedRecreate.active();
}

void QRhiUniform::recreateResource() {
	mUniformBlock.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer,mDataSize));
	mUniformBlock->create();

	mSampler.reset(RHI->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();

	for (auto& textureItem : mTextureList) {
		textureItem->texture.reset(RHI->newTexture(QRhiTexture::RGBA8, textureItem->image.size(), 1));
		textureItem->texture->create();
		textureItem->bNeedUpdate.active();
	}

	for (auto& dataParam : mDataList) {
		dataParam->bNeedUpdate.active();
	}
}

void QRhiUniform::updateResource(QRhiResourceUpdateBatch* batch) {
	if (bNeedRecreate.receive()) {
		recreateResource();
		for (auto& ref : mRef) {
			ref->bNeedRecreatePipeline.active();
		}
	}

	for (auto& dataParam : mDataList) {
		if (dataParam->bNeedUpdate.receive()) {
			batch->updateDynamicBuffer(mUniformBlock.get(), dataParam->offsetInByte, dataParam->sizeInByte, dataParam->var.data());
		}
	}

	for (auto& textureParam : mTextureList) {
		if (textureParam->bNeedUpdate.receive()) {
			batch->uploadTexture(textureParam->texture.get(), textureParam->image);
		}
	}
}

QList<std::shared_ptr<QRhiUniform::ParamDescBase>> QRhiUniform::getParamList() {
	return mParamNameMap.values();
}

QRhiUniform::UniformInfo QRhiUniform::getUniformInfo(uint8_t bindingOffset /*= 0*/, QString blockName /*= "UBO"*/, QRhiShaderResourceBinding::StageFlag stage /*= QRhiShaderResourceBinding::FragmentStage*/) {
	QRhiUniform::UniformInfo info;
	QString uniformCode;
	if (!mDataList.isEmpty()) {
		info.bindings << QRhiShaderResourceBinding::uniformBuffer(bindingOffset, stage, mUniformBlock.get());
		uniformCode = "layout(binding = " + QString::number(bindingOffset) + ") uniform " + blockName + "Block { \n";
		for (auto& param : mDataList) {
			uniformCode += QString("    %1 %2;\n").arg(param->getTypeName()).arg(param->name);
		}
		uniformCode += "}" + blockName + ";\n";
		bindingOffset++;
	}
	for (auto& texture : mTextureList) {
		info.bindings << QRhiShaderResourceBinding::sampledTexture(bindingOffset, stage, texture->texture.get(), mSampler.get());
		uniformCode += QString("layout (binding = %1) uniform %2 %3;\n").arg(bindingOffset).arg("sampler2D").arg(texture->name);
		bindingOffset++;
	}
	info.uniformDefineCode = uniformCode.toLocal8Bit();
	info.bindingOffset = bindingOffset;
	return info;
}
