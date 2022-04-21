#ifndef QRenderSystem_h__
#define QRenderSystem_h__

#include <QObject>
#include "QRenderWindow.h"
#include "Renderer\QRenderer.h"

#define TheRenderSystem (QRenderSystem::instance())
#define RHI (TheRenderSystem->getRHI())

class IRenderable;

class QRenderSystem: public QObject {
	friend class ISceneRenderPass;
public:
	static QRenderSystem* instance();
	static QShader createShaderFromCode(QShader::Stage stage, const char* code);

	void init();
	void shutdown();
	bool hasRequestQuit();
	void requestUpdate();

	int getSceneSampleCount();
	QVector<QRhiGraphicsPipeline::TargetBlend> getSceneBlendStates();
	QRhiRenderPassDescriptor* getSceneRenderPassDescriptor();
	QRenderWindow* window();

	QRhi* getRHI();
	void setupRHI(std::shared_ptr<QRhi> val) { mRHI = val; }

	QRenderer* renderer();
	bool isEnableDebug();
	void setEnableDebug(bool var) { mEnableDebug = var; }

	qint64 getRenderDurationMs() const;
private:
	QRenderSystem();
private:
	QRenderWindow* mWindow;
	std::shared_ptr<QRhi> mRHI;
	std::shared_ptr<QRenderer> mRenderer;
	bool mEnableDebug = false;
};


#endif // QRenderSystem_h__
