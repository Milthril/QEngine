#ifndef FileListWidget_h__
#define FileListWidget_h__

#include "QListWidget"
#include "QDir"
#include "QFuture"
#include "QFileSystemWatcher"

class FileListWidget;


class FileListWidget :public QListWidget {
	Q_OBJECT
public:
	friend class FileTaskThread;
	FileListWidget();
	~FileListWidget();
	void setCurrentDir(QString dir);
	void setCurrentDirAndSearch(QString dir, QString keyword);
	void updateCurrentItem();
	void addElideItem();
	void searchFile(const QString& keyword);
	void submitItemCount();
Q_SIGNALS:
	void newMessage(QString);
private:
	QDir currentDir_;
	QFileSystemWatcher fileWatcher_;
protected:
	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dragMoveEvent(QDragMoveEvent* e) override;
	virtual void dropEvent(QDropEvent* event) override;
	virtual void startDrag(Qt::DropActions supportedActions) override;
};

#endif // FileListWidget_h__
