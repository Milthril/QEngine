#ifndef SceneTreeWidget_h__
#define SceneTreeWidget_h__

#include "QTreeWidget"
#include "Scene\QScene.h"

class SceneTreeWidget :public QTreeWidget {
	Q_OBJECT
public:
	SceneTreeWidget(std::shared_ptr<QScene> scene);
private:
	void createUI();
	void updateUI();
private:
	std::shared_ptr<QScene> mScene;
	QTreeWidgetItem* mRoot;
Q_SIGNALS:
	void objectChanged(QObject* obejct);
};

#endif // SceneTreeWidget_h__
