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
}

void QMaterialProxy::updateResource(QRhiResourceUpdateBatch* batch)
{
	for (auto& params : mMaterial->mParams) {
		if (params.needUpdate) {
			batch->updateDynamicBuffer(mUniformBlock.get(), params.offsetInByte, params.sizeInByte, mMaterial->mData.data() + params.offsetInByte);
			params.needUpdate = false;
		}
	}
}

QMaterialProxy::MaterialShaderInfo QMaterialProxy::getMaterialShaderInfo(uint8_t bindingOffset /*= 0*/) {
	QMaterialProxy::MaterialShaderInfo info;
	info.bindings << QRhiShaderResourceBinding::uniformBuffer(bindingOffset, QRhiShaderResourceBinding::FragmentStage, mUniformBlock.get());
	QString uniformCode = "layout(binding = " + QString::number(bindingOffset) + ") uniform MaterialInfo{ \n";
	for (auto& param : mMaterial->mParams) {
		uniformCode += QString("    %1 %2;\n").arg(QMaterial::typeIdToShaderType(param.typeId)).arg(param.name);
	}
	uniformCode += "}material;\n";
	bindingOffset++;
	for (auto& key : mTextureMap.keys()) {
		uniformCode += QString("layout (binding = %1) uniform %2 %3; ").arg(bindingOffset++).arg("sampler2D").arg(key);
	}
	info.uniformCode = uniformCode.toLocal8Bit();
	info.shadingCode = mMaterial->mShadingCode;
	return info;
}