#include "QConsolePanel.h"
#include <QDateTime>


QConsolePanel::QConsolePanel() {

}

void QConsolePanel::addMessage(QtMsgType type, const QMessageLogContext&, const QString& text) {
	QDateTime currentDateTime = QDateTime::currentDateTime();
	QString newText = currentDateTime.toString("[hh:mm:ss] ") + text;
	switch (type) {
	case QtDebugMsg:
	case QtCriticalMsg:
		break;
	case QtWarningMsg:
		newText = QString("<font color = \"#F2DFAE\" >%1</font>").arg(newText);
		break;
	case QtInfoMsg:
		newText = QString("<font color = \"#00FF00\" >%1</font>").arg(newText);
		break;
	case QtFatalMsg:
		newText = QString("<font color = red>%1</font>").arg(newText);
		break;
	default:
		break;
	}
	append(newText);
}

