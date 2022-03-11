#ifndef DesktopApplication_h__
#define DesktopApplication_h__
#include <QApplication>
#include <QSystemTrayIcon>

class QLocalServer;

class DesktopApplication : public QApplication {
	Q_OBJECT
public:
	DesktopApplication(QString iconPath, int argc, char** argv);
	~DesktopApplication();
	bool isRunning();
protected:
	void initLocalConnection();
	bool notify(QObject* receiver, QEvent* event) override;
private slots:
	void newLocalConnection();
	void newLocalServer();
protected:
	bool bRunning;
	QLocalServer* localServer_;      // 本地socket Server
	QString serverName_;             // 服务名称
public:
	QSystemTrayIcon* systemIcon_;
	void setAutoStart(bool bAutoStart);
	bool isAutoStart();
};

#endif // DesktopApplication_h__
