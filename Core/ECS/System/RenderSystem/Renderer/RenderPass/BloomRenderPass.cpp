#include "BloomRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"

BloomMerageRenderPass::BloomMerageRenderPass(){}


void BloomMerageRenderPass::setupSrcTexutre(QRhiTexture* texture) {
	mSrcTexture = texture;
}

void BloomMerageRenderPass::setupBloomTexutre(QRhiTexture* texture) {
	mBloomTexture = texture;
}

void BloomMerageRenderPass::compile() {
	mRT.colorAttachment.reset(RHI->newTexture(QRhiTexture::RGBA32F, mSrcTexture->pixelSize(), 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
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
	QShader vs = QRenderSystem::createShaderFromCode(QShader::VertexStage, R"(#version 450
layout (location = 0) out vec2 vUV;
out gl_PerVertex{
	vec4 gl_Position;
};
void main() {
	vUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(vUV * 2.0f - 1.0f, 0.0f, 1.0f);
}
)");

	QShader fs = QRenderSystem::createShaderFromCode(QShader::FragmentStage, R"(#version 450
layout (binding = 0) uniform sampler2D uSrcTexture;
layout (binding = 1) uniform sampler2D uBloomTexture;
layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 outFragColor;
void main() {
	vec4 srcColor = texture(uSrcTexture, vUV);
	vec4 bloomColor = texture(uBloomTexture, vUV);

	float gamma = 1.0f;
	vec3 mapped = vec3(1.0)-exp(-(srcColor.rgb+bloomColor.rgb));
	mapped = pow(mapped,vec3(1.0f/gamma));
	outFragColor = vec4(max(mapped,srcColor.rgb),1.0);
}
)");

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							   });

	QRhiVertexInputLayout inputLayout;

	mBindings.reset(RHI->newShaderResourceBindings());
	mBindings->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mSrcTexture,mSampler.get()),
		QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mBloomTexture,mSampler.get())
						   });

	mBindings->create();
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(mRT.renderTarget->renderPassDescriptor());
	mPipeline->create();
}

void BloomMerageRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {
	cmdBuffer->beginPass(mRT.renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, mRT.renderTarget->pixelSize().width(), mRT.renderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);
	cmdBuffer->endPass();
}
