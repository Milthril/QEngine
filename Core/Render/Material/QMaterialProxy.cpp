#include "QMaterialProxy.h"
#include "QMaterial.h"

QMaterialProxy::QMaterialProxy(QMaterial* material, std::shared_ptr<QRhi> rhi)
	: mMaterial(material)
	, mRhi(rhi)

{
}

void QMaterialProxy::recreateResource()
{
	mUniformBlock.reset(mRhi->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, mMaterial->mData.size()));
	mUniformBlock->create();

	mSampler.reset(mRhi->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();

	mTextureMap.clear();
	for (auto& texture : mMaterial->mTexture) {
		QRhiSPtr<QRhiTexture> tex;
		tex.reset(mRhi->newTexture(QRhiTexture::RGBA8, texture.image.size(), 1));
		mTextureMap[texture.name] = tex;
		tex->create();
	}
}

void QMaterialProxy::updateResource(QRhiResourceUpdateBatch* batch)
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

QMaterialProxy::MaterialShaderInfo QMaterialProxy::getMaterialShaderInfo(uint8_t bindingOffset /*= 0*/) {
	QMaterialProxy::MaterialShaderInfo info;
	
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
	info.uniformCode = uniformCode.toLocal8Bit();
	info.shadingCode = mMaterial->mShadingCode;
	return info;
}