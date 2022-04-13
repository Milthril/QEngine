#ifndef ImporterTask_h__
#define ImporterTask_h__
#include <QString>
#include "QDir"
#include <QMutex>

class ImporterTask {
public:
	ImporterTask(QString filePath, QDir destDir)
		: mFilePath(filePath)
		, mDestDir(destDir)
	{}
	void executable();
private:
	void resolveModel();
	void resolveImage();
	QString getVaildPath(QString path);
private:
	QString mFilePath;
	QDir mDestDir;
	QMutex mMutex;
};


#endif // ImporterTask_h__
