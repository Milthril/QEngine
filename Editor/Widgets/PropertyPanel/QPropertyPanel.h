#ifndef QPropertyPanel_h__
#define QPropertyPanel_h__

#include <QTreeWidget>

class QPropertyPanel :public QTreeWidget {
public:
	QPropertyPanel(QObject* object = nullptr);
	QObject* getObject() const;
	void setObject(QObject* val);
private:
	void updatePanel();
private:
	QObject* object_ = nullptr;
protected:
	void showEvent(QShowEvent* event) override;
};

#endif // QPropertyPanel_h__
