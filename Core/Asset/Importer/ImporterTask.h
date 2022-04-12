#ifndef ImporterTask_h__
#define ImporterTask_h__

#include <QString>
#include "QDir"

class ImporterTask {
public:
	ImporterTask(QString filePath, QDir destDir)
		: mFilePath(filePath)
		, mDestDir(destDir)
	{}
	void executable();
private:
	void resolveModel();
private:
	QString mFilePath;
	QDir mDestDir;
};


#endif // ImporterTask_h__
