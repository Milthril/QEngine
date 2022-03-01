#include "QBloomPainter.h"

QBloomPainter::QBloomPainter(QSceneRenderer* renderer)
	:mRenderer(renderer)
{
}

void QBloomPainter::drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget)
{
	createOrResize(renderTarget->pixelSize());
	if (!mPipelineH) {
		initRhiResource();
	}

	//cmdBuffer->beginPass(renderTarget, QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	//cmdBuffer->setGraphicsPipeline(mPipeline.get());
	//cmdBuffer->setShaderResources(mBindings.get());
	//cmdBuffer->draw(4);
	//cmdBuffer->endPass();
}

void QBloomPainter::initRhiResource()
{
	auto mRhi = mRenderer->getRhi();
	mSampler.reset(mRhi->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mSampler->create();

	mPipelineH.reset(mRhi->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipelineH->setTargetBlends({ blendState });
	mPipelineH->setSampleCount(mRenderer->getSampleCount());
	QShader vs = QSceneRenderer::createShaderFromCode(QShader::VertexStage, R"(#version 450
	layout (location = 0) out vec2 outUV;
	out gl_PerVertex{
		vec4 gl_Position;
	};
	void main() {
		outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
		gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
	}
	)");

	QShader fsH = QSceneRenderer::createShaderFromCode(QShader::FragmentStage, R"(#version 450
	layout (binding = 0) uniform sampler2D samplerColor;
	layout (location = 0) in vec2 inUV;
	layout (location = 0) out vec4 outFragColor;
	void main() {
		outFragColor = texture(samplerColor, inUV);
	}
	)");
	mPipelineH->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fsH }
	});
	QRhiVertexInputLayout inputLayout;
	mBindingsH.reset(mRhi->newShaderResourceBindings());
	mBindingsV->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBloomRT[0].colorAttachment.get(),mSampler.get())
	});
	mBindingsH->create();
	mPipelineH->setVertexInputLayout(inputLayout);
	mPipelineH->setShaderResourceBindings(mBindingsH.get());
	mPipelineH->setRenderPassDescriptor(renderPassDesc.get());
	mPipelineH->create();

	//-------------------------------------------------------------

	mPipelineV.reset(mRhi->newGraphicsPipeline());
	mPipelineV->setTargetBlends({ blendState });
	mPipelineV->setSampleCount(mRenderer->getSampleCount());

	QShader fsV = QSceneRenderer::createShaderFromCode(QShader::FragmentStage, R"(#version 450
	layout (binding = 0) uniform sampler2D samplerColor;
	layout (location = 0) in vec2 inUV;
	layout (location = 0) out vec4 outFragColor;
	void main() {
		outFragColor = texture(samplerColor, inUV);
	}
	)");
	mPipelineH->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fsV }
	});
	mBindingsV.reset(mRhi->newShaderResourceBindings());
	mBindingsV->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBloomRT[1].colorAttachment.get(),mSampler.get())
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
	auto mRhi = mRenderer->getRhi();
	for (int i = 0; i < 2; i++) {
		mBloomRT[i].colorAttachment.reset(mRhi->newTexture(QRhiTexture::RGBA8, size, mRenderer->getSampleCount(), QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
		mBloomRT[i].colorAttachment->create();
		mBloomRT[i].renderTarget.reset(mRhi->newTextureRenderTarget({ mBloomRT[i].colorAttachment.get() }));
		mBloomRT[i].renderTarget->create();
	}
	renderPassDesc.reset(mBloomRT[0].renderTarget->newCompatibleRenderPassDescriptor());

	if (mBindingsH) {
		mBindingsH->destroy();
		mBindingsH->setBindings({
			QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBloomRT[0].colorAttachment.get(),mSampler.get())
		});
		mBindingsH->create();
	}

	if (mBindingsV) {
		mBindingsV->destroy();
		mBindingsV->setBindings({
			QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mBloomRT[1].colorAttachment.get(),mSampler.get())
		});
		mBindingsV->create();
	}
}