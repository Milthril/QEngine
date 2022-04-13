#ifndef QScenePanel_h__
#define QScenePanel_h__

#include "QTreeWidget"
#include "ECS\QWorld.h"

class QScenePanel :public QTreeWidget {
	Q_OBJECT
public:
	QScenePanel(std::shared_ptr<QWorld> world);
private:
	void createUI();
	void updateUI();
private:
	std::shared_ptr<QWorld> mWorld;
	QTreeWidgetItem* mRoot;
Q_SIGNALS:
	void entityChanged(QEntity* obejct);
protected:
	void showEvent(QShowEvent* event) override;

};

#endif // QScenePanel_h__
