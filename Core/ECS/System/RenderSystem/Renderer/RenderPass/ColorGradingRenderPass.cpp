#include "ColorGradingRenderPass.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "Asset\GAssetMgr.h"
#include "QDir"

ColorGradingRenderPass::ColorGradingRenderPass() {
	QDir lutDir(TheAssetMgr->assetDir().filePath("LUT"));
	mLutCombo.addItem("Null");
	for (auto file : lutDir.entryInfoList({}, QDir::Files)) {
		if (file.suffix().compare("PNG", Qt::CaseInsensitive)==0
			|| file.suffix().compare("JPG", Qt::CaseInsensitive) == 0
			|| file.suffix().compare("JPEG", Qt::CaseInsensitive) == 0
			) {
			mLutCombo.addItem(file.baseName());
			mNameToPath[file.baseName()] = file.filePath();
		}
	}
}

void ColorGradingRenderPass::compile() {
	mRT.colorAttachment.reset(RHI->newTexture(QRhiTexture::RGBA8, mInputTextures[InputTextureSlot::Color]->pixelSize() , 1, QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
	mRT.colorAttachment->create();
	mRT.renderTarget.reset(RHI->newTextureRenderTarget({ mRT.colorAttachment.get() }));
	mRT.renderPassDesc.reset(mRT.renderTarget->newCompatibleRenderPassDescriptor());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->setRenderPassDescriptor(mRT.renderPassDesc.get());
	mRT.renderTarget->create();
	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, sizeof(Param)));
	mUniformBuffer->create();
	mSampler.reset(RHI->newSampler(QRhiSampler::Linear,
				   QRhiSampler::Linear,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));

	mSampler->create();
	mOutputTextures[OutputTextureSlot::Result] = mRT.colorAttachment.get();
	recreatePipeline();
}

void ColorGradingRenderPass::recreatePipeline() {
	auto iter = mNameToPath.find(mLutCombo.getCurrentItem());
	if (iter != mNameToPath.end()) {
		QImage image(iter.value());
		if(image.width() % image.height() == 0) {
			mParam.LUTBlockNum =  image.width() / image.height();
			mParam.LUTBlockSize = image.height();
			mLUT = image.convertedTo(QImage::Format_RGBA8888);
		}
	}

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
	mBindings.reset(RHI->newShaderResourceBindings());
	QShader vs = QRenderSystem::createShaderFromCode(QShader::VertexStage, vsCode.arg(RHI->isYUpInNDC() ? "	vUV.y = 1 - vUV.y;" : "").toLocal8Bit());
	QShader fs;
	if (mLUT.isNull()) {
		fs = QRenderSystem::createShaderFromCode(QShader::FragmentStage, R"(#version 450
		layout (binding = 0) uniform sampler2D uTexture;
		layout (location = 0) in vec2 vUV;
		layout (location = 0) out vec4 outFragColor;
		void main(){
			outFragColor = texture(uTexture,vUV);
		}
		)");
		mBindings->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage, mInputTextures[InputTextureSlot::Color], mSampler.get())
							   });
	}
	else {
		fs = QRenderSystem::createShaderFromCode(QShader::FragmentStage, R"(#version 450
		layout (binding = 0) uniform sampler2D uTexture;
		layout (binding = 1) uniform sampler2D uLUT;
		layout (binding = 2 ) uniform UniformBuffer{
			int LUTBlockNum;
			float LUTBlockSize;
		}UBO;
		layout (location = 0) in vec2 vUV;
		layout (location = 0) out vec4 outFragColor;
		void main(){
			vec3 inputColor = texture(uTexture,vUV).rgb;
			vec2 LUTSize = textureSize(uLUT, 0);

			float blueOffset= inputColor.b * UBO.LUTBlockNum;

			int index0 =  min(int(blueOffset),UBO.LUTBlockNum - 2);
			vec2 uv0 = (vec2(index0 * UBO.LUTBlockSize,0) + inputColor.rg * UBO.LUTBlockSize)/LUTSize;
			vec3 color0 = texture(uLUT,uv0).rgb;

			int index1 = index0+1;
			vec2 uv1 = (vec2(index1 * UBO.LUTBlockSize,0) + inputColor.rg * UBO.LUTBlockSize)/LUTSize;
			vec3 color1 = texture(uLUT,uv1).rgb;
			
			vec3 result = mix(color0,color1,blueOffset - index0);

			outFragColor = vec4(result,1.0);
		}
		)");
		mLUTTexture.reset(RHI->newTexture(QRhiTexture::RGBA8, mLUT.size()));
		mLUTTexture->create();
		mBindings->setBindings({
			QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage, mInputTextures[InputTextureSlot::Color], mSampler.get()),
			QRhiShaderResourceBinding::sampledTexture(1,QRhiShaderResourceBinding::FragmentStage, mLUTTexture.get(), mSampler.get()),
			QRhiShaderResourceBinding::uniformBuffer(2,QRhiShaderResourceBinding::FragmentStage,mUniformBuffer.get())
							   });
	}
	mBindings->create();

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

void ColorGradingRenderPass::execute(QRhiCommandBuffer* cmdBuffer) {
	if (!mLUT.isNull()) {
		QRhiResourceUpdateBatch* batch = RHI->nextResourceUpdateBatch();
		batch->uploadTexture(mLUTTexture.get(), mLUT);
		batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(Param), &mParam);
		cmdBuffer->resourceUpdate(batch);
		mLUT = {};
	}
	cmdBuffer->beginPass(mRT.renderTarget.get(), QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f), { 1.0f, 0 });
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, mRT.renderTarget->pixelSize().width(), mRT.renderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources(mBindings.get());
	cmdBuffer->draw(4);
	cmdBuffer->endPass();
}


QCombo ColorGradingRenderPass::getLUT() const {
	return mLutCombo;
}

void ColorGradingRenderPass::setLUT(QCombo val) {
	mLutCombo = val;
	recreatePipeline();
}

