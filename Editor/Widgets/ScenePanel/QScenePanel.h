#ifndef QScenePanel_h__
#define QScenePanel_h__

#include "QTreeWidget"
#include "Scene\QScene.h"

class QScenePanel :public QTreeWidget {
	Q_OBJECT
public:
	QScenePanel(std::shared_ptr<QScene> scene);
private:
	void createUI();
	void updateUI();
private:
	std::shared_ptr<QScene> mScene;
	QTreeWidgetItem* mRoot;
Q_SIGNALS:
	void objectChanged(QObject* obejct);
protected:
	void showEvent(QShowEvent* event) override;

};

#endif // QScenePanel_h__
