#ifndef QAssetImporter_h__
#define QAssetImporter_h__

#include <QThread>
#include <QObject>
#include "ImporterTask.h"
#include "Asset\IAsset.h"

class QAssetImpoerter: public QObject {
public:
	QAssetImpoerter();
	~QAssetImpoerter();
	void import(QString path,QDir destDir);
private:
	Q_INVOKABLE void threadFunc();
private:
	QThread mThread;
	QList<std::shared_ptr<ImporterTask>> mTaskList;
};



#endif // QAssetImporter_h__
