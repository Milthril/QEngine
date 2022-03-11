#ifndef AEROWIDGET_H
#define AEROWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class CustomHeaderBar;

class FramelessWidget : public QWidget
{
	Q_OBJECT
public:
	FramelessWidget(const QString& title = "", QWidget* parent = 0);
	~FramelessWidget();
	void setWindowTitle(QString title);
	CustomHeaderBar* headerBar;
	QVBoxLayout vLayout;
protected:
	virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
	virtual void mousePressEvent(QMouseEvent* e)override;
	virtual void mouseMoveEvent(QMouseEvent* e)override;
	virtual void mouseReleaseEvent(QMouseEvent* event)override;
	virtual void paintEvent(QPaintEvent* event)override;
	static int boundaryWidth;
	QPoint clickPos;
private:
	// QWidget interface
protected:
	virtual void showEvent(QShowEvent* event) override;
};

#endif // FRAMELESSWIDGET_H