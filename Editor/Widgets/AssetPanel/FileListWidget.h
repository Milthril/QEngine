#ifndef FileListWidget_h__
#define FileListWidget_h__

#include "QListWidget"
#include "QDir"
#include "QFuture"

class FileListWidget;

class FileListWidegtThreadTask :public QObject {
	Q_OBJECT
public:
	FileListWidegtThreadTask(FileListWidget* widget);
	~FileListWidegtThreadTask();
	void search(const QString& keyword);
	void updateFileItems();
	void clearIconCache();
	QIcon getIcon(const QString& filePath);
private:
	Q_INVOKABLE void searchInternal(const QString& keyword);
	Q_INVOKABLE void updateFileItemInternal();
	Q_INVOKABLE void cacheIcon(const QString filePath);
	void addCacheFile(const QFileInfo& info);
	void submitCache();
Q_SIGNALS:
	void fileSearched(QFileInfo fileInfo);
private:
	QMutex mutex;
	QThread* thread_;
	FileListWidget* widget_;
	QList<QFileInfo> cacheList_;
	QHash<QString, QIcon> iconCacheMap_;
	const int cacheSize = 10;
	const int searchLimited = 5000000;
};

class FileListWidget :public QListWidget {
	Q_OBJECT
public:
	friend class FileListWidegtThreadTask;
	FileListWidget();
	~FileListWidget();
	void setCurrentDir(QString dir);
	void setCurrentDirAndSearch(QString dir, QString keyword);
	void addElideItem();
	void searchFile(const QString& keyword);
	void submitItemCount();
Q_SIGNALS:
	void newMessage(QString);
private:
	QDir currentDir_;
	FileListWidegtThreadTask threadTask_;
};

#endif // FileListWidget_h__
