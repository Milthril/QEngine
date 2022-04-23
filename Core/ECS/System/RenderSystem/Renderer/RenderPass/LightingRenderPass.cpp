#include "LightingRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "ECS\Component\LightComponent\ILightComponent.h"
#include "QEngineCoreApplication.h"
#include "ECS\Component\QCameraComponent.h"

LightingRenderPass::LightingRenderPass(){}

void LightingRenderPass::setupBaseColorTexutre(QRhiTexture* texture) {
	mBaseColorTexture = texture;
}

void LightingRenderPass::setupPositionTexutre(QRhiTexture* texture) {
	mPositionTexture = texture;
}

void LightingRenderPass::setupNormalMetalnessTexutre(QRhiTexture* texture) {
	mNormal_MetalnessTexture = texture;
}

void LightingRenderPass::setupTangentRoughnessTexutre(QRhiTexture* texture) {
	mTangent_RoughnessTexture = texture;
}

void LightingRenderPass::addLightItem(ILightComponent* item) {
	mLightItemList << item;
	rebuildLight();
}

void LightingRenderPass::removeLightItem(ILightComponent* item) {
	mLightItemList.removeOne(item);
	rebuildLight();
}

void LightingRenderPass::compile() {
	mRT.colorAttachment.reset(RHI->newTexture(QRhiTexture::RGBA32F, mBaseColorTexture->pixelSize(), 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.colorAttachment->create();
	mRT.renderTarget.reset(RHI->newTextureRenderTarget({ mRT.colorAttachment.get() }));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();

	mSampler.reset(RHI->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();
	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, sizeof(UniformBlock)));
	mUniformBuffer->create();
	rebuildLight();
}

void LightingRenderPass::rebuildLight() {
	mPipeline.reset(RHI->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipeline->setTargetBlends({ blendState });
	mPipeline->setSampleCount(mRT.renderTarget->sampleCount());

	mBindings.reset(RHI->newShaderResourceBindings());
	QVector<QRhiShaderResourceBinding> shaderBindings;
	shaderBindings << QRhiShaderResourceBinding::sampledTexture(0, QRhiShaderResourceBinding::FragmentStage, mBaseColorTexture, mSampler.get());
	shaderBindings << QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, mPositionTexture, mSampler.get());
	shaderBindings << QRhiShaderResourceBinding::sampledTexture(2, QRhiShaderResourceBinding::FragmentStage, mNormal_MetalnessTexture, mSampler.get());
	shaderBindings << QRhiShaderResourceBinding::sampledTexture(3, QRhiShaderResourceBinding::FragmentStage, mTangent_RoughnessTexture, mSampler.get());
	shaderBindings << QRhiShaderResourceBinding::uniformBuffer(4, QRhiShaderResourceBinding::FragmentStage, mUniformBuffer.get(), 0, sizeof(UniformBlock));

	QByteArray lightDefineCode;
	QByteArray lightingCode;

	uint8_t bindingOffset = 5;
	for (int i = 0; i < mLightItemList.size(); i++) {
		auto& lightItem = mLightItemList[i];

		QString name = QString("Light%1").arg(i);
		const auto& lightInfo = lightItem->getUniformInfo(bindingOffset, name);

		lightDefineCode += lightInfo.uniformDefineCode;
		lightingCode += lightItem->getLightingCode(name);

		bindingOffset = lightInfo.bindingOffset;

		shaderBindings << lightInfo.bindings;
	}
	mBindings->setBindings(shaderBindings.begin(), shaderBindings.end());
	mBindings->create();

	QString vsCode = R"(#version 450
layout (location = 0) out vec2 vUV;
out gl_PerVertex{
	vec4 gl_Position;
};
void main() {
	vUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(vUV * 2.0f - 1.0f, 0.0f, 1.0f);
	%1
}
)";
	QShader vs = QRenderSystem::createShaderFromCode(QShader::VertexStage, vsCode.arg(RHI->isYUpInNDC() ? "	vUV.y = 1 - vUV.y;" : "").toLocal8Bit());

	QByteArray fsCode = QString(R"(#version 450
layout (binding = 0) uniform sampler2D uBaseColorTexture;
layout (binding = 1) uniform sampler2D uPositionTexture;
layout (binding = 2) uniform sampler2D uNormal_MetalnessTexture;
layout (binding = 3) uniform sampler2D uTangent_RoughnessTexture;
layout (binding = 4) uniform UniformBlock{
	vec3 eyePosition;
}UBO;
%1

layout (location = 0) in vec2 vUV;

layout (location = 0) out vec4 outFragColor;

const float PI = 3.141592;
const float Epsilon = 0.00001;
// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 Fdielectric = vec3(0.04);

float ndfGGX(float cosLh, float roughness){
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;
	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k){
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness){
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta){
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

struct AnalyticalLight {
	vec3 direction;
	vec3 radiance;
};

void main() {
	vec3 baseColor =	texture(uBaseColorTexture, vUV).rgb;
	float metalness =	texture(uNormal_MetalnessTexture, vUV).a;
	float roughness =	texture(uTangent_RoughnessTexture, vUV).a;
	vec3 position =		texture(uPositionTexture, vUV).rgb;
	vec3 N =			normalize(texture(uNormal_MetalnessTexture, vUV).rgb);

	vec3 Lo = normalize(UBO.eyePosition - position);
	
	float cosLo = max(0.0, dot(N, Lo));
		
	vec3 Lr = 2.0 * cosLo * N - Lo;

	vec3 F0 = mix(Fdielectric, baseColor, metalness);

	vec3 directLighting = vec3(0);
	%2
	outFragColor = vec4(vec3(0.5)*baseColor+directLighting,1);
}
)").arg(lightDefineCode).arg(lightingCode).toLocal8Bit();

	QShader fs = QRenderSystem::createShaderFromCode(QShader::FragmentStage, fsCode);

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
	});

	QRhiVertexInputLayout inputLayout;
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(mRT.renderTarget->renderPassDescriptor());
	mPipeline->create();
}

void LightingRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {
	QRhiResourceUpdateBatch* resUpdateBatch = RHI->nextResourceUpdateBatch();
	for (auto& lightItem : mLightItemList) {
		lightItem->updateResource(resUpdateBatch);
	}
	UniformBlock block;
	block.eyePosition = TheEngine->world()->getCurrentCamera()->getPosition();
	resUpdateBatch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(UniformBlock), &block);

	cmdBuffer->beginPass(mRT.renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 }, resUpdateBatch);
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, mRT.renderTarget->pixelSize().width(), mRT.renderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources();
	cmdBuffer->draw(4);
	cmdBuffer->endPass();
}
