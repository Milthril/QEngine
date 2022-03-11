#include "QRhiUniformProxy.h"
#include "QRhiUniform.h"

QRhiUniformProxy::QRhiUniformProxy(QRhiUniform* material)
	: mMaterial(material)
{
}

void QRhiUniformProxy::recreateResource(QRhi* rhi)
{
	mUniformBlock.reset(rhi->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, mMaterial->mData.size()));
	mUniformBlock->create();

	mSampler.reset(rhi->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();

	mTextureMap.clear();
	for (auto& texture : mMaterial->mTexture) {
		QRhiSPtr<QRhiTexture> tex;
		tex.reset(rhi->newTexture(QRhiTexture::RGBA8, texture.image.size(), 1));
		mTextureMap[texture.name] = tex;
		tex->create();
	}
}

void QRhiUniformProxy::updateResource(QRhiResourceUpdateBatch* batch)
{
	for (auto& params : mMaterial->mParams) {
		if (params.needUpdate) {
			batch->updateDynamicBuffer(mUniformBlock.get(), params.offsetInByte, params.sizeInByte, mMaterial->mData.data() + params.offsetInByte);
			params.needUpdate = false;
		}
	}
	for (auto& params : mMaterial->mTexture) {
		if (params.needUpdate) {
			batch->uploadTexture(mTextureMap[params.name].get(), params.image);
			params.needUpdate = false;
		}
	}
}

QRhiUniformProxy::UniformInfo QRhiUniformProxy::getUniformInfo(uint8_t bindingOffset /*= 0*/) {
	QRhiUniformProxy::UniformInfo info;
	QString uniformCode;
	if (!mMaterial->mParams.isEmpty()) {
		info.bindings << QRhiShaderResourceBinding::uniformBuffer(bindingOffset, QRhiShaderResourceBinding::FragmentStage, mUniformBlock.get());
		uniformCode = "layout(binding = " + QString::number(bindingOffset) + ") uniform MaterialInfo{ \n";
		for (auto& param : mMaterial->mParams) {
			uniformCode += QString("    %1 %2;\n").arg(param.getTypeName()).arg(param.name);
		}
		uniformCode += "}material;\n";
		bindingOffset++;
	}
	for (auto& key : mTextureMap.keys()) {
		info.bindings << QRhiShaderResourceBinding::sampledTexture(bindingOffset, QRhiShaderResourceBinding::FragmentStage, mTextureMap[key].get(), mSampler.get());
		uniformCode += QString("layout (binding = %1) uniform %2 %3;\n").arg(bindingOffset).arg("sampler2D").arg(key);
		bindingOffset++;
	}
	info.uniformDefineCode = uniformCode.toLocal8Bit();
	return info;
}