#ifndef QWidgetListPanel_h__
#define QWidgetListPanel_h__

#include <QScrollArea>
#include <QPushButton>

class QVBoxLayout; 
class WinCloseButton;

class QWidgetListItem :public QWidget{
	Q_OBJECT
public:
	using QWidget::QWidget;
	QWidgetListItem(QString name ,QWidget *widget);
Q_SIGNALS:
	void requsetRemoveItem(QWidgetListItem*);
private:
	QPushButton mTitleButton;
	WinCloseButton* mCloseButton;
	QWidget* mWidget;
};

class QWidgetListPanel: public QScrollArea {
	Q_OBJECT
public:
	QWidgetListPanel();
	void clear();
	void addWidget(QString name, QWidget* widget);
	void removeWidget(QWidget* widget);
Q_SIGNALS:
	void widgetRemoved(QWidget*);
private:
	QWidget* mBody;
	QVBoxLayout* vLayout;
};


#endif // QWidgetListPanel_h__
