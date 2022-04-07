#ifndef QEngine_h__
#define QEngine_h__

#include "RHI\QRhiWindow.h"
#include "Scene\QScene.h"
#include <QApplication>
#include <QDir>

#if defined(Engine)
#undef Engine
#endif
#define Engine (static_cast<QEngine *>(QEngine::instance()))
#define RHI (static_cast<QEngine *>(QEngine::instance()))->getRHI()

class QEngine :public QApplication
{
	Q_OBJECT
public:
	QEngine(int argc, char** argv, bool enableDebug = false);
	const std::shared_ptr<QScene>& scene();
	const std::shared_ptr<QRhiWindow>& window();
	const std::shared_ptr<ISceneRenderer>& renderer();
	const QDir& assetDir() const { return mAssetDir; }
	void execGame();
	QRhi* getRHI();
Q_SIGNALS:
	void requestUpdatePropertyPanel();
protected:
	virtual void init();
	virtual void update();
private:
	std::shared_ptr<QRhiWindow> mWindow;
	std::shared_ptr<QScene> mScene;
	std::shared_ptr<ISceneRenderer>  mRenderer;
	QDir mAssetDir;
};

#endif // QEngine_h__
