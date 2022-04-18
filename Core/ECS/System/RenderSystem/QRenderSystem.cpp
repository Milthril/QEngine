#include "QRenderSystem.h"
#include "IRenderable.h"
#include "private\qshaderbaker_p.h"
#include "Renderer\IRenderer.h"
#include "Renderer\RenderPass\ISceneRenderPass.h"
#include "Renderer\DefaultRenderer\QDefaultRenderer.h"

QRenderSystem::QRenderSystem()
	: mWindow(new QRenderWindow(QRhi::Implementation::Vulkan))
	, mRenderer(std::make_shared<QDefaultRenderer>() ){
}

QRenderSystem* QRenderSystem::instance() {
	static QRenderSystem ins;
	return &ins;
}

QShader QRenderSystem::createShaderFromCode(QShader::Stage stage, const char* code) {
	QShaderBaker baker;
	baker.setGeneratedShaderVariants({ QShader::StandardShader });
	baker.setGeneratedShaders({
		QShaderBaker::GeneratedShader{QShader::Source::SpirvShader,QShaderVersion(100)},
		QShaderBaker::GeneratedShader{QShader::Source::GlslShader,QShaderVersion(430)},
		QShaderBaker::GeneratedShader{QShader::Source::MslShader,QShaderVersion(12)},
		QShaderBaker::GeneratedShader{QShader::Source::HlslShader,QShaderVersion(50)},
	});

	baker.setSourceString(code, stage);
	QShader shader = baker.bake();
	if (!shader.isValid()) {
		qWarning(code);
		qWarning(baker.errorMessage().toLocal8Bit());
	}
	return shader;
}


void QRenderSystem::init() {
	mWindow->show();
	mWindow->waitExposed();
}

void QRenderSystem::shutdown() {
	mRenderer.reset();
}

bool QRenderSystem::hasRequestQuit() {
	return mWindow->hasClosed();
}

void QRenderSystem::requestUpdate() {
	mWindow->requestUpdate();
}

void QRenderSystem::addRenderItem(IRenderable* comp) {
	mRenderer->mScenePass->mRenderItemList << comp;
}

void QRenderSystem::removeRenderItem(IRenderable* comp) {
	mRenderer->mScenePass->mRenderItemList.removeOne(comp);
}

int QRenderSystem::getSceneSampleCount() {
	return mRenderer->getScenePass()->getSampleCount();
}

QVector<QRhiGraphicsPipeline::TargetBlend> QRenderSystem::getSceneBlendStates() {
	return mRenderer->getScenePass()->getBlendStates();
}

QRhiRenderPassDescriptor* QRenderSystem::getSceneRenderPassDescriptor() {
	return mRenderer->getScenePass()->getRenderPassDescriptor();
}

QRenderWindow* QRenderSystem::window() {
	return mWindow;
}

QRhi* QRenderSystem::getRHI() {
	return mWindow->mRhi.get();
}

IRenderer* QRenderSystem::renderer() {
	return mRenderer.get();
}

bool QRenderSystem::isEnableDebug() {
	return mEnableDebug;
}

