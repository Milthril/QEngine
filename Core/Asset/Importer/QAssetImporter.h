#ifndef QAssetImporter_h__
#define QAssetImporter_h__

#include <QThread>
#include <QObject>
#include "ImporterTask.h"
#include "Asset\IAsset.h"

class QAssetImpoerter: public QObject {
public:
	static QAssetImpoerter* instance();
	~QAssetImpoerter();
	void import(QString path,QDir destDir);
	
private:
	QAssetImpoerter();
	Q_INVOKABLE void threadFunc();
private:
	QThread mThread;
	QList<std::shared_ptr<ImporterTask>> mTaskList;
};



#endif // QAssetImporter_h__
