#ifndef SceneTreeWidget_h__
#define SceneTreeWidget_h__

#include "QTreeWidget"
#include "Scene\QScene.h"

class SceneTreeWidget :public QTreeWidget {
	SceneTreeWidget(std::shared_ptr<QScene> scene);
private:
	std::shared_ptr<QScene> mScene;
};

#endif // SceneTreeWidget_h__
