#include "DesktopApplication.h"
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>
#include <QSettings>
#include <QDir>

DesktopApplication::DesktopApplication(QString iconPath, int argc, char** argv)
	: QApplication(argc, argv)
	, serverName_(QFileInfo(QCoreApplication::applicationFilePath()).fileName())
	, systemIcon_(new QSystemTrayIcon(QIcon(iconPath)))
	, bRunning(false)
{
	initLocalConnection();
	setQuitOnLastWindowClosed(false);
	systemIcon_->show();
}

DesktopApplication::~DesktopApplication()
{
	systemIcon_->hide();
}

bool DesktopApplication::isRunning()
{
	return bRunning;
}

void DesktopApplication::initLocalConnection()
{
	bRunning = false;
	QLocalSocket socket;
	socket.connectToServer(serverName_);
	if (socket.waitForConnected(500))
	{
		bRunning = true;
		// 将启动参数发送到服务端
		QTextStream stream(&socket);
		QStringList args = QApplication::arguments();
		if (args.count() > 1)
			stream << args.last();
		else
			stream << QString();
		stream.flush();
		socket.waitForBytesWritten();
		QApplication::quit();
		return;
	}
	//连接不上服务器，就创建一个
	newLocalServer();
}

bool DesktopApplication::notify(QObject* receiver, QEvent* event)
{
	bool done = true;
	try {
		done = QApplication::notify(receiver, event);
	}
	catch (const std::exception& ex) {
	}
	catch (...) {
	}
	return done;
}

void DesktopApplication::newLocalConnection()
{
	QLocalSocket* socket = localServer_->nextPendingConnection();
	if (!socket)
		return;
	socket->waitForReadyRead(1000);
	QTextStream stream(socket);
	//其他处理
	//if (mainWindow != NULL)
	//{
	//	//激活窗口
	//	QString path = stream.readAll();
	//	DesignWindow::getInstance().file->importFile(path);
	//	mainWindow->raise();
	//	mainWindow->display();
	//}
	delete socket;
}

void DesktopApplication::newLocalServer()
{
	localServer_ = new QLocalServer(this);
	connect(localServer_, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));
	if (!localServer_->listen(serverName_))
	{
		// 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
		if (localServer_->serverError() == QAbstractSocket::AddressInUseError)
		{
			QLocalServer::removeServer(serverName_); // <-- 重点
			localServer_->listen(serverName_); // 再次监听
		}
	}
}

void DesktopApplication::setAutoStart(bool bAutoStart)
{
	QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
				  QSettings::NativeFormat);
	QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
	QString strAppName = QFileInfo(strAppPath).baseName();
	if (bAutoStart)
		reg.setValue(strAppName, strAppPath + QString(" -AutoStart"));
	else
		reg.remove(strAppName);
}

bool DesktopApplication::isAutoStart()
{
	QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
				  QSettings::NativeFormat);
	QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
	QString strAppName = QFileInfo(strAppPath).baseName();
	return !reg.value(strAppName).toString().isEmpty();
}
