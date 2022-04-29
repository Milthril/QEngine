#include "ForwardRenderPass.h"
#include "QEngineCoreApplication.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"
#include "ECS/System/RenderSystem/Renderer/QRenderer.h"
#include "ECS/System/RenderSystem/RHI/IRenderable.h"

void ForwardRenderPass::compile() {		//创建默认的RT
	QSize frameSize = mRenderer->getFrameSize();
	if (mRT.atBaseColor && mRT.atBaseColor->pixelSize() == frameSize)
		return;

	QVector<QRhiColorAttachment> mColorAttachmentList;
	mBlendStates.clear();
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = false;

	mRT.atBaseColor.reset(RHI->newTexture(QRhiTexture::RGBA32F, frameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.atBaseColor->create();
	mBlendStates << blendState;
	mColorAttachmentList << QRhiColorAttachment(mRT.atBaseColor.get());

	if (TheRenderSystem->isEnableDebug()) {
		mBlendStates << blendState;
		QRhiColorAttachment debugAttachment;
		mRT.atDebugId.reset(RHI->newTexture(QRhiTexture::RGBA8, frameSize, 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
		mRT.atDebugId->create();
		debugAttachment.setTexture(mRT.atDebugId.get());
		mColorAttachmentList << debugAttachment;
	}

	QRhiTextureRenderTargetDescription RTDesc;
	RTDesc.setColorAttachments(mColorAttachmentList.begin(), mColorAttachmentList.end());
	mRT.atDepth.reset(RHI->newRenderBuffer(QRhiRenderBuffer::DepthStencil, frameSize, 1, {},QRhiTexture::D32F));
	mRT.atDepth->create();
	RTDesc.setDepthStencilBuffer(mRT.atDepth.get());
	mRT.renderTarget.reset(RHI->newTextureRenderTarget(RTDesc));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();


	mSampler.reset(RHI->newSampler(QRhiSampler::Nearest,
				   QRhiSampler::Filter::Nearest,
				   QRhiSampler::Filter::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));

	mSampler->create();

	mCopyPipeline.reset(RHI->newGraphicsPipeline());
	mCopyPipeline->setTargetBlends(mBlendStates.begin(),mBlendStates.end());
	mCopyPipeline->setSampleCount(1);
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

	QString fsCode = R"(#version 450
layout (binding = 0) uniform sampler2D uBaseColor;
layout (binding = 1) uniform sampler2D uDepth;
%1

layout (location = 0) in vec2 vUV;

layout (location = 0) out vec4 outBaseColor;
%2

void main() {
	outBaseColor = texture(uBaseColor, vUV);
	gl_FragDepth = texture(uDepth, vUV).r;
	%3
}
)";
	QList<QRhiShaderResourceBinding> mBindingList;
	mBindingList << QRhiShaderResourceBinding::sampledTexture(0, QRhiShaderResourceBinding::FragmentStage, mInputTextures[InputTextureSlot::Color], mSampler.get());
	mBindingList << QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, mInputTextures[InputTextureSlot::Depth], mSampler.get());
	if (TheRenderSystem->isEnableDebug()) {
		mBindingList << QRhiShaderResourceBinding::sampledTexture(2, QRhiShaderResourceBinding::FragmentStage, mInputTextures[InputTextureSlot::DeferDebugId], mSampler.get());
		fsCode = fsCode
			.arg("layout(binding = 2) uniform sampler2D uDebugId;")
			.arg("layout(location = 1) out vec4 outDebugId;")
			.arg("outDebugId = texture(uDebugId, vUV);");
	}
	else {
		fsCode = fsCode.arg("").arg("").arg("");
	}
	QShader fs = QRenderSystem::createShaderFromCode(QShader::FragmentStage, fsCode.toLocal8Bit());

	mCopyPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							   });

	mBindings.reset(RHI->newShaderResourceBindings());
	mBindings->setBindings(mBindingList.begin(),mBindingList.end());
	mBindings->create();
	mCopyPipeline->setDepthTest(true);
	mCopyPipeline->setDepthWrite(true);
	mCopyPipeline->setVertexInputLayout(QRhiVertexInputLayout());
	mCopyPipeline->setShaderResourceBindings(mBindings.get());
	mCopyPipeline->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mCopyPipeline->create();

	mOutputTextures[OutputTextureSlot::Output] = mRT.atBaseColor.get();
	mOutputTextures[OutputTextureSlot::DebugId] = mRT.atDebugId.get();
}

ForwardRenderPass::ForwardRenderPass() {
}

QVector<QRhiGraphicsPipeline::TargetBlend> ForwardRenderPass::getBlendStates()
{
	return mBlendStates;
}

QRhiRenderTarget* ForwardRenderPass::getRenderTarget()
{
	return mRT.renderTarget.get();
}

void ForwardRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {

	const auto& forwardList = mRenderer->getForwardItemList();
	for (auto& item : forwardList) {
		item->updatePrePass(cmdBuffer);
	}
	QRhiResourceUpdateBatch* resUpdateBatch = RHI->nextResourceUpdateBatch();
	for (auto& item : forwardList) {
		if (item->bNeedRecreateResource.receive()) {
			item->recreateResource();
			item->uploadResource(resUpdateBatch);
		}
		item->updateResourcePrePass(resUpdateBatch);
		if (item->bNeedRecreatePipeline.receive()) {
			item->recreatePipeline();
		}
	}
	cmdBuffer->beginPass(getRenderTarget(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 }, resUpdateBatch);
	QRhiViewport viewport(0, 0, getRenderTarget()->pixelSize().width(), getRenderTarget()->pixelSize().height());

	for (auto& item : forwardList) {
		if (item->bNeedRecreatePipeline.receive()) {
			item->recreatePipeline();
		}
		item->renderInPass(cmdBuffer, viewport);
	}

	cmdBuffer->setGraphicsPipeline(mCopyPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);

	cmdBuffer->endPass();
}
