#ifndef QEntityPanel_h__
#define QEntityPanel_h__

#include <QPushButton>
#include <QLabel>
#include "QLineEdit"

class QEntity;
class QWidgetListPanel;

class QEntityPanel :public QWidget {
public:
	QEntityPanel(QEntity* entity = nullptr);
	QEntity* getEntity() const;
	void setEntity(QEntity* val);
private:
	void recreatePanel();
private:
	QEntity* mEntity = nullptr;
	QLineEdit lbName;
	QWidgetListPanel* mWidgetPanel;
	QPushButton btAddComponent;
protected:
	void showEvent(QShowEvent* event) override;
};

#endif // QEntityPanel_h__
