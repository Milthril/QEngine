#ifndef FileListWidget_h__
#define FileListWidget_h__

#include "QListWidget"
#include "QDir"
#include "QFuture"
#include "QFileSystemWatcher"

class FileListWidget;

class FileTaskThread :public QObject {
	Q_OBJECT
public:
	FileTaskThread(FileListWidget* widget);
	~FileTaskThread();
	void search(const QString& keyword);
	void updateFileItems();
	void clearIconCache();
	QIcon getIcon(const QString& filePath);
private:
	Q_INVOKABLE void searchThreadTask(const QString& keyword);
	Q_INVOKABLE void updateWidgetTaskThread();
	Q_INVOKABLE void updateIconTaskThread(const QString filePath);
	void addCacheFile(const QFileInfo& info);
	void submitCache();
	void waitFinished();
	void onActive();
	void onFinished();
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
	std::atomic<bool> bIsRunning = false;
	std::atomic<bool> bRequestQuit = false;
	std::promise<bool> bFinished;
};

class FileListWidget :public QListWidget {
	Q_OBJECT
public:
	friend class FileTaskThread;
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
	QFileSystemWatcher fileWatcher_;
	FileTaskThread threadTask_;
protected:
	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dragMoveEvent(QDragMoveEvent* e) override;
	virtual void dropEvent(QDropEvent* event) override;
};

#endif // FileListWidget_h__
