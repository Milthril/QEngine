#include "QBloomPainter.h"

QBloomPainter::QBloomPainter(std::shared_ptr<QRhi> rhi)
	: mRhi(rhi)
	, mWriteBackPainter(rhi)
	, mMeragePainter(rhi)
	, mPixelSelector(rhi, R"(
void main() {
	vec4 color = texture(uTexture, vUV);
	float value = max(max(color.r,color.g),color.b);
	outFragColor = (1-step(value,1.0f)) * color;
}
)")
{
	setBloomSize(20);
}

void QBloomPainter::drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> inputTexture, QRhiRenderTarget* outputTarget) {
	createOrResize(outputTarget->pixelSize());
	if (!mPipelineH) {
		initRhiResource();
	}

	mPixelSelector.drawCommand(cmdBuffer, inputTexture, mBloomRT[0].renderTarget.get());
	if (bNeedUpdateBloomState) {
		QRhiResourceUpdateBatch* copyBatch = mRhi->nextResourceUpdateBatch();
		copyBatch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(BloomState), &mBloomState);
		cmdBuffer->resourceUpdate(copyBatch);
	}

	for (int i = 0; i < mBoommIter; i++) {
		cmdBuffer->beginPass(mBloomRT[1].renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
		cmdBuffer->setGraphicsPipeline(mPipelineH.get());
		cmdBuffer->setShaderResources(mBindingsH.get());
		cmdBuffer->setViewport(QRhiViewport(0, 0, outputTarget->pixelSize().width(), outputTarget->pixelSize().height()));
		cmdBuffer->draw(4);
		cmdBuffer->endPass();

		cmdBuffer->beginPass(mBloomRT[0].renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
		cmdBuffer->setGraphicsPipeline(mPipelineV.get());
		cmdBuffer->setShaderResources(mBindingsV.get());
		cmdBuffer->setViewport(QRhiViewport(0, 0, outputTarget->pixelSize().width(), outputTarget->pixelSize().height()));
		cmdBuffer->draw(4);
		cmdBuffer->endPass();
	}
	mMeragePainter.drawCommand(cmdBuffer, inputTexture, mBloomRT[0].colorAttachment, outputTarget);
}

void QBloomPainter::setBloomSize(int size)
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

void QBloomPainter::initRhiResource()
{
	mSampler.reset(mRhi->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();

	mUniformBuffer.reset(mRhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, sizeof(BloomState)));
	mUniformBuffer->create();

	mPipelineH.reset(mRhi->newGraphicsPipeline());
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
	mBindingsH.reset(mRhi->newShaderResourceBindings());
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

	mPipelineV.reset(mRhi->newGraphicsPipeline());
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
	mBindingsV.reset(mRhi->newShaderResourceBindings());
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

void QBloomPainter::createOrResize(QSize size)
{
	if (mBloomRT[0].colorAttachment && mBloomRT[0].colorAttachment->pixelSize() == size)
		return;
	for (int i = 0; i < 2; i++) {
		mBloomRT[i].colorAttachment.reset(mRhi->newTexture(QRhiTexture::RGBA32F, size, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
		mBloomRT[i].colorAttachment->create();
		mBloomRT[i].renderTarget.reset(mRhi->newTextureRenderTarget({ mBloomRT[i].colorAttachment.get() }));
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

QBloomMeragePainter::QBloomMeragePainter(std::shared_ptr<QRhi> rhi)
	:mRhi(rhi)
{
}

void QBloomMeragePainter::initRhiResource(QRhiRenderPassDescriptor* renderPassDesc, QRhiRenderTarget* renderTarget, QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom)
{
	mSrcTexture = src;
	mBloomTexture = bloom;
	mSampler.reset(mRhi->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();
	mPipeline.reset(mRhi->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipeline->setTargetBlends({ blendState });
	mPipeline->setSampleCount(renderTarget->sampleCount());
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

	QShader fs = QSceneRenderer::createShaderFromCode(QShader::FragmentStage, R"(#version 450
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

	mBindings.reset(mRhi->newShaderResourceBindings());
	mBindings->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mSrcTexture.get(),mSampler.get()),
		QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mBloomTexture.get(),mSampler.get())
	});

	mBindings->create();
	mPipeline->setVertexInputLayout(inputLayout);
	mPipeline->setShaderResourceBindings(mBindings.get());
	mPipeline->setRenderPassDescriptor(renderPassDesc);
	mPipeline->create();
}

void QBloomMeragePainter::updateTexture(QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom)
{
	mSrcTexture = src;
	mBloomTexture = bloom;
	if (mBindings) {
		mBindings->destroy();
		mBindings->setBindings({
			QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mSrcTexture.get(),mSampler.get()),
			QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage,mBloomTexture.get(),mSampler.get())
		});
		mBindings->create();
	}
}

void QBloomMeragePainter::drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiSPtr<QRhiTexture> src, QRhiSPtr<QRhiTexture> bloom, QRhiRenderTarget* renderTarget)
{
	auto it = renderTarget->renderPassDescriptor();
	if (src != mSrcTexture) {
		if (!mSrcTexture)
			initRhiResource(renderTarget->renderPassDescriptor(), renderTarget, src, bloom);
		else
			updateTexture(src, bloom);
	}
	cmdBuffer->beginPass(renderTarget, QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, renderTarget->pixelSize().width(), renderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);
	cmdBuffer->endPass();
}