#include "BlurPass.h"
#include "Renderer\QSceneRenderer.h"
#include "QEngine.h"

BlurPass::BlurPass() {
	setBloomSize(10);
}

void BlurPass::makeBlur(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> inputTexture)
{
	if (!mPipelineH) {
		initRhiResource();
	}
	if (bNeedUpdateBloomState.receive()) {
		QRhiResourceUpdateBatch* copyBatch = RHI->nextResourceUpdateBatch();
		copyBatch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(BloomState), &mBloomState);
		cmdBuffer->resourceUpdate(copyBatch);
	}
	for (int i = 0; i < mBoommIter; i++) {
		cmdBuffer->beginPass(mBloomRT[1].renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
		cmdBuffer->setGraphicsPipeline(mPipelineH.get());
		cmdBuffer->setShaderResources(mBindingsH.get());
		cmdBuffer->setViewport(QRhiViewport(0, 0, inputTexture->pixelSize().width(), inputTexture->pixelSize().height()));
		cmdBuffer->draw(4);
		cmdBuffer->endPass();

		cmdBuffer->beginPass(mBloomRT[0].renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
		cmdBuffer->setGraphicsPipeline(mPipelineV.get());
		cmdBuffer->setShaderResources(mBindingsV.get());
		cmdBuffer->setViewport(QRhiViewport(0, 0, inputTexture->pixelSize().width(), inputTexture->pixelSize().height()));
		cmdBuffer->draw(4);
		cmdBuffer->endPass();
	}
}

void BlurPass::setBloomSize(int size)
{
	if (size <= 0 || size == mBloomState.size || size >= std::size(mBloomState.weight))
		return;
	mBloomState.size = size;
	float sum = 1, s = 1;
	mBloomState.weight[size - 1] = 1;
	for (int i = size - 2; i >= 0; i--) {
		mBloomState.weight[i] = (mBloomState.weight[i + 1] + s);
		++s;
		sum += mBloomState.weight[i] * 2;
	}
	mBloomState.weight[0] /= sum / 2;
	for (int i = 1; i < size; i++) {
		mBloomState.weight[i] /= sum;
	}
	bNeedUpdateBloomState = true;
}

void BlurPass::initRhiResource()
{
	mSampler.reset(RHI->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();

	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, sizeof(BloomState)));
	mUniformBuffer->create();

	mPipelineH.reset(RHI->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = false;
	mPipelineH->setTargetBlends({ blendState });
	mPipelineH->setSampleCount(1);
	QShader vs = QSceneRenderer::createShaderFromCode(QShader::VertexStage, R"(#version 450
	layout (location = 0) out vec2 vUV;
	out gl_PerVertex{
		vec4 gl_Position;
	};
	void main() {
		vUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
		gl_Position = vec4(vUV * 2.0f - 1.0f, 0.0f, 1.0f);
	}
	)");
	QShader fsH = QSceneRenderer::createShaderFromCode(QShader::FragmentStage, R"(#version 450
layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 outFragColor;
layout (binding = 0) uniform sampler2D uTexture;

layout (binding = 1 ) uniform BloomState{
	int size;
	vec4 weight[10];
}bloomState;

void main(){
	vec2 tex_offset = 1.0 / textureSize(uTexture, 0); // gets size of single texel
	vec4 raw = texture(uTexture, vUV);
	vec4 result = raw * bloomState.weight[0][0]; // current fragment's contribution
	for(int i = 1; i < bloomState.size; ++i){
		const float weight = bloomState.weight[i/4][i%4];
		result += texture(uTexture, vUV + vec2(tex_offset.x * i, 0.0)) * weight;
		result += texture(uTexture, vUV - vec2(tex_offset.x * i, 0.0)) * weight;
	}
    outFragColor = result;
}
	)");
	mPipelineH->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fsH }
								});
	QRhiVertexInputLayout inputLayout;
	mBindingsH.reset(RHI->newShaderResourceBindings());
	mBindingsH->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBloomRT[0].colorAttachment.get(),mSampler.get()),
		QRhiShaderResourceBinding::uniformBuffer(1,QRhiShaderResourceBinding::FragmentStage,mUniformBuffer.get())
							});
	mBindingsH->create();
	mPipelineH->setVertexInputLayout(inputLayout);
	mPipelineH->setShaderResourceBindings(mBindingsH.get());
	mPipelineH->setRenderPassDescriptor(renderPassDesc.get());
	mPipelineH->create();

	//-------------------------------------------------------------

	mPipelineV.reset(RHI->newGraphicsPipeline());
	mPipelineV->setTargetBlends({ blendState });
	mPipelineV->setSampleCount(1);

	QShader fsV = QSceneRenderer::createShaderFromCode(QShader::FragmentStage, R"(#version 450
layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 outFragColor;
layout (binding = 0) uniform sampler2D uTexture;

layout (binding = 1 ) uniform BloomState{
	int size;
	vec4 weight[10];
}bloomState;

void main(){
	vec2 tex_offset = 1.0 / textureSize(uTexture, 0); // gets size of single texel
	vec4 raw = texture(uTexture, vUV);
	vec4 result = raw * bloomState.weight[0][0]; // current fragment's contribution
	for(int i = 1; i < bloomState.size; ++i){
		const float weight = bloomState.weight[i/4][i%4];
		result += texture(uTexture, vUV + vec2(0.0,tex_offset.y * i)) * weight;
		result += texture(uTexture, vUV - vec2(0.0,tex_offset.y * i)) * weight;
	}
    outFragColor = result;
}
	)");
	mPipelineV->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fsV }
								});
	mBindingsV.reset(RHI->newShaderResourceBindings());
	mBindingsV->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBloomRT[1].colorAttachment.get(),mSampler.get()),
		QRhiShaderResourceBinding::uniformBuffer(1,QRhiShaderResourceBinding::FragmentStage,mUniformBuffer.get())
							});
	mBindingsV->create();
	mPipelineV->setVertexInputLayout(inputLayout);
	mPipelineV->setShaderResourceBindings(mBindingsV.get());
	mPipelineV->setRenderPassDescriptor(renderPassDesc.get());
	mPipelineV->create();
}

void BlurPass::createOrResize(QSize size)
{
	if (mBloomRT[0].colorAttachment && mBloomRT[0].colorAttachment->pixelSize() == size)
		return;
	for (int i = 0; i < 2; i++) {
		mBloomRT[i].colorAttachment.reset(RHI->newTexture(QRhiTexture::RGBA32F, size, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
		mBloomRT[i].colorAttachment->create();
		mBloomRT[i].renderTarget.reset(RHI->newTextureRenderTarget({ mBloomRT[i].colorAttachment.get() }));
	}
	renderPassDesc.reset(mBloomRT[0].renderTarget->newCompatibleRenderPassDescriptor());

	for (int i = 0; i < 2; i++) {
		mBloomRT[i].renderTarget->setRenderPassDescriptor(renderPassDesc.get());
		mBloomRT[i].renderTarget->create();
	}

	if (mBindingsH) {
		mBindingsH->destroy();
		mBindingsH->setBindings({
			QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBloomRT[0].colorAttachment.get(),mSampler.get()),
			QRhiShaderResourceBinding::uniformBuffer(1,QRhiShaderResourceBinding::FragmentStage,mUniformBuffer.get())
								});
		mBindingsH->create();
	}

	if (mBindingsV) {
		mBindingsV->destroy();
		mBindingsV->setBindings({
			QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBloomRT[1].colorAttachment.get(),mSampler.get()),
			QRhiShaderResourceBinding::uniformBuffer(1,QRhiShaderResourceBinding::FragmentStage,mUniformBuffer.get())
								});
		mBindingsV->create();
	}
}