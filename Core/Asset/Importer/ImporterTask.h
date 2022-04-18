#ifndef ImporterTask_h__
#define ImporterTask_h__

#include <QString>
#include <QDir>

class ImporterTask {
public:
	ImporterTask(){}

	virtual void executable() = 0;

	void setFilePath(QString val) { mFilePath = val; }
	void setDestDir(QDir val) { mDestDir = val; }

protected:
	QString mFilePath;
	QDir mDestDir;
};

#endif // ImporterTask_h__
