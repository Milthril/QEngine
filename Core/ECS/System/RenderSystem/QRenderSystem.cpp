#include "QRenderSystem.h"
#include "private\qshaderbaker_p.h"

QRenderSystem::QRenderSystem()
	: mWindow(new QRenderWindow(QRhi::Implementation::Vulkan))
	, mRenderer(std::make_shared<QRenderer>()){
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
		//QShaderBaker::GeneratedShader{QShader::Source::HlslShader,QShaderVersion(60)},
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
	mRHI.reset();
}

bool QRenderSystem::hasRequestQuit() {
	return mWindow->hasClosed();
}

void QRenderSystem::requestUpdate() {
	mWindow->requestUpdate();
}

QRenderWindow* QRenderSystem::window() {
	return mWindow;
}

QRhi* QRenderSystem::getRHI() {
	return mRHI.get();
}

QRenderer* QRenderSystem::renderer() {
	return mRenderer.get();
}

bool QRenderSystem::isEnableDebug() {
	return mEnableDebug;
}

qint64 QRenderSystem::getRenderDurationMs() const {
	return mWindow->mDurationMs;;
}

