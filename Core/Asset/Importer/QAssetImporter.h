#ifndef QAssetImporter_h__
#define QAssetImporter_h__

#include <QThread>
#include <QObject>
#include "ImporterTask.h"

class QAssetImpoerter: public QObject {
public:
	static QAssetImpoerter* instance();
	void import(QString path,QDir destDir);
private:
	QAssetImpoerter();
	Q_INVOKABLE void threadFunc();
private:
	QThread mThread;
	QList<std::shared_ptr<ImporterTask>> mTaskList;
};

#endif // QAssetImporter_h__
