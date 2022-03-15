#ifndef SceneTreeWidget_h__
#define SceneTreeWidget_h__

#include "QTreeWidget"
#include "Scene\QScene.h"

class SceneTreeWidget :public QTreeWidget {
public:
	SceneTreeWidget(std::shared_ptr<QScene> scene);
private:
	void createUI();
private:
	std::shared_ptr<QScene> mScene;
	std::shared_ptr<QTreeWidgetItem> mRoot;
};

#endif // SceneTreeWidget_h__
