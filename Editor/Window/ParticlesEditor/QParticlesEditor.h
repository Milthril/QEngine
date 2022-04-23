#ifndef QParticlesEditor_h__
#define QParticlesEditor_h__

#include "KDDockWidgets\DockWidget.h"
#include "Asset\PartcleSystem\ParticleSystem.h"

class GLSLEditor;
class UniformPanel;
class QPushButton;
class QPropertyPanel;

class QParticlesEditor :public KDDockWidgets::DockWidget {
public:
	static QParticlesEditor* instance();
	void edit(std::shared_ptr<Asset::ParticleSystem> system);
private:
	QParticlesEditor();
private:
	std::shared_ptr<Asset::ParticleSystem> mSystem;
private:
	QPropertyPanel* mEmitterPanel;
	UniformPanel* mUniformPanel;
	GLSLEditor* editor;
	QPushButton* btCompile;
protected:
	void closeEvent(QCloseEvent*) override;

};

#endif // QParticlesEditor_h__
