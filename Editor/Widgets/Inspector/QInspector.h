#ifndef QInspector_h__
#define QInspector_h__

#include "QWidget"

class QVBoxLayout; 

class QInspector :public QWidget {
public:
	QInspector(QObject* object = nullptr);
	QObject* getObject() const;
	void setObject(QObject* val);
private:
	void recreatePanel();
private:
	QObject* mObject = nullptr;
	QWidget* mWidget = nullptr;
	QVBoxLayout* mLayout = nullptr;
};

#endif // QInspector_h__
