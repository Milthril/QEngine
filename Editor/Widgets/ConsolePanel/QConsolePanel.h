#ifndef QConsolePanel_h__
#define QConsolePanel_h__

#include <QTextBrowser>

class QConsolePanel :public QTextBrowser {
public:
	QConsolePanel();
	void addMessage(QtMsgType, const QMessageLogContext&, const QString&);
};

#endif // QConsolePanel_h__
