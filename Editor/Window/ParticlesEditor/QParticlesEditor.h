#ifndef QParticlesEditor_h__
#define QParticlesEditor_h__

#include "KDDockWidgets\DockWidget.h"

class GLSLEditor;
class UniformPanel;
class QPushButton;
class QParticleSystem;

class QParticlesEditor :public KDDockWidgets::DockWidget {
public:
	static QParticlesEditor* instance();
	void edit(std::shared_ptr<QParticleSystem> system);
private:
	QParticlesEditor();
private:
	std::shared_ptr<QParticleSystem> mSystem;
private:
	UniformPanel* mUniformPanel;
	GLSLEditor* editor;
	QPushButton* btCompile;
};

#endif // QParticlesEditor_h__
