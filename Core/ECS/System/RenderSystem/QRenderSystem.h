#ifndef QRenderSystem_h__
#define QRenderSystem_h__

#include <QObject>
#include "QRenderWindow.h"

#define RHI (QRenderSystem::instance()->getRHI())

class IRenderer;
class IRenderable;
class ISceneRenderPass;

class QRenderSystem: public QObject {
	friend class ISceneRenderPass;
public:
	static QRenderSystem* instance();
	static QShader createShaderFromCode(QShader::Stage stage, const char* code);

	void init();
	void shutdown();
	bool hasRequestQuit();
	void requestUpdate();
	void addRenderItem(IRenderable* comp);
	void removeRenderItem(IRenderable* comp);

	int getSceneSampleCount();
	QVector<QRhiGraphicsPipeline::TargetBlend>  getSceneBlendStates();
	QRhiRenderPassDescriptor* getSceneRenderPassDescriptor();
	QRenderWindow* window();

	QRhi* getRHI();
	void setupRHI(std::shared_ptr<QRhi> val) { mRHI = val; }

	IRenderer* renderer();
	bool isEnableDebug();
	void setEnableDebug(bool var) { mEnableDebug = var; }

	qint64 getRenderDurationMs() const;
private:
	QRenderSystem();
private:
	QRenderWindow* mWindow;
	std::shared_ptr<QRhi> mRHI;
	std::shared_ptr<IRenderer> mRenderer;
	bool mEnableDebug = false;
};


#endif // QRenderSystem_h__
