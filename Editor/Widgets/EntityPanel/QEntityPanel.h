#ifndef QEntityPanel_h__
#define QEntityPanel_h__

#include <QToolBox>
#include <QPushButton>
#include <QLabel>

class QEntity;

class QEntityPanel :public QWidget {
public:
	QEntityPanel(QEntity* entity = nullptr);
	QEntity* getEntity() const;
	void setEntity(QEntity* val);
private:
	void clearToolBox();
	void recreatePanel();
private:
	QEntity* mEntity = nullptr;
	QLabel lbName;
	QToolBox mToolBox;
	QPushButton btAddComponent;
protected:
	void showEvent(QShowEvent* event) override;
};

#endif // QEntityPanel_h__
