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
Q_SIGNALS:
	void entityChanged(QEntity* obejct);
private:
	QPoint mClickPos;
protected:
	void showEvent(QShowEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // QScenePanel_h__
