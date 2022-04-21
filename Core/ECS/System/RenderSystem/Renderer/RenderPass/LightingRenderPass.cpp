#include "LightingRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"

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
	mPipeline.reset(RHI->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipeline->setTargetBlends({ blendState });
	mPipeline->setSampleCount(mRT.renderTarget->sampleCount());

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

	QShader fs = QRenderSystem::createShaderFromCode(QShader::FragmentStage, R"(#version 450
layout (binding = 0) uniform sampler2D uBaseColorTexture;
layout (binding = 1) uniform sampler2D uPositionTexture;
layout (binding = 2) uniform sampler2D uNormal_MetalnessTexture;
layout (binding = 3) uniform sampler2D uTangent_RoughnessTexture;
layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 outFragColor;
void main() {
	outFragColor = texture(uNormal_MetalnessTexture, vUV);
}
)");

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
	});

	QRhiVertexInputLayout inputLayout;

	mBindings.reset(RHI->newShaderResourceBindings());

	mBindings->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBaseColorTexture,mSampler.get()),
		QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mPositionTexture,mSampler.get()),
		QRhiShaderResourceBinding::sampledTexture(2,QRhiShaderResourceBinding::FragmentStage,mNormal_MetalnessTexture,mSampler.get()),
		QRhiShaderResourceBinding::sampledTexture(3,QRhiShaderResourceBinding::FragmentStage,mTangent_RoughnessTexture,mSampler.get()),
	});

	mBindings->create();
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(mRT.renderTarget->renderPassDescriptor());
	mPipeline->create();
}

void LightingRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {
	cmdBuffer->beginPass(mRT.renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, mRT.renderTarget->pixelSize().width(), mRT.renderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);
	cmdBuffer->endPass();
}
