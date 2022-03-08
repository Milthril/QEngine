#include "QMaterialProxy.h"
#include "QMaterial.h"

QMaterialProxy::QMaterialProxy(QMaterial* material, std::shared_ptr<QRhi> rhi)
{
}

void QMaterialProxy::recreateResource()
{
}

void QMaterialProxy::uploadResource()
{
}

void QMaterialProxy::updateResource()
{
}

QMaterialProxy::MaterialShaderInfo QMaterialProxy::getMaterialShaderInfo(uint8_t bindingOffset /*= 0*/)
{
	QMaterialProxy::MaterialShaderInfo info;
	info.bindings << QRhiShaderResourceBinding::uniformBuffer(bindingOffset, QRhiShaderResourceBinding::FragmentStage, mUniformBlock.get());
	QString uniformCode = "layout(binding = " + QString::number(bindingOffset) + ") uniform MaterialInfo{ \n";
	for (auto& param : mMaterial->mParams) {
		uniformCode += QString("    %1 %2;\n").arg(0).arg(param.name);
	}
	uniformCode += "}material;\n";
	bindingOffset++;
	for (auto& key : mTextureMap.keys()) {
		uniformCode += QString("layout (binding = %1) uniform %2 %3; ").arg(bindingOffset++).arg(0).arg(key);
	}
	info.uniformCode = uniformCode.toLocal8Bit();
	info.shadingCode = mMaterial->mShadingCode;
	return info;
}